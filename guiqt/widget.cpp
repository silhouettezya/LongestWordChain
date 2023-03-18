#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <Qlabel>
#include <QTextStream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <sys/stat.h>
#include <windows.h>
#include <QLibrary>
#include<QTimer>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
        myTimer = new QTimer(this);
    connect(myTimer,&QTimer::timeout,[=](){
            static float i = 0;
            i= i + 0.00000000000000001;
            ui->lcd->display(i);
        });

}

Widget::~Widget()
{
    delete ui;
}
void Widget::loadFile() {
    QString filename = QFileDialog::getOpenFileName(this,
        NULL, NULL, "Text files (*.txt);; *.*");
    if (filename.isEmpty()) {
        return;
    }
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "文件打开失败", "你选择的文件不存在或者拒绝访问");
        return;
    }
    QTextStream in(&file);
    QString inputStr = in.readAll();
    ui->input->setPlainText(inputStr);
}
int Widget::Qstring2words(QString s, char* word_buf, char** words) {
    const int BEGIN = 0;
    const int READING = 1;

    std::unordered_set<std::string> filter;
    std::vector<std::string> ret;

    char c;
    char buf[1024];
    int state = BEGIN;
    int top = 0;
    for (QChar qchar : s) {
        c = qchar.toLatin1();
        if (state == BEGIN) {
            if ('a' <= c && c <= 'z') {
                *buf = (char)c;
                top = 1;
                state = READING;
            }
            else if ('A' <= c && c <= 'Z') {
                *buf = (char)(c + 0x20);
                top = 1;
                state = READING;
            }
        }
        else { // state == READING
            if ('a' <= c && c <= 'z') {
                buf[top] = c;
                top++;
            }
            else if ('A' <= c && c <= 'Z') {
                buf[top] = (char)(c + 0x20);
                top++;
            }
            else {
                state = BEGIN;
                buf[top] = 0;
                if (top > 1) {
                    filter.insert(buf);
                }
            }
        }
    }
    if (state == READING && top > 1) {
        buf[top] = 0;
        filter.insert(buf);
    }
    top = 0;
    int num = 0;
    for (const auto& w : filter) {
        const char* wd = w.c_str();
        words[num++] = word_buf + top;
        int i = 0;
        while (wd[i]) {
            word_buf[top++] = wd[i++];
        }
        word_buf[top++] = 0;
    }
    return (int)filter.size();

}
void Widget::startCalc() {

    const int MAX_WORD_NUM = 10020;
    const int MAX_CHAIN_NUM = 20020;
    char head = ui->head->currentIndex()>0 ? ui->head->currentIndex() + 'a' - 1 : '\0';
    char tail = ui->tail->currentIndex()>0 ? ui->tail->currentIndex() + 'a' - 1 : '\0';
    char forbid = ui->forbid->currentIndex()>0 ? ui->forbid->currentIndex() + 'a' - 1 : '\0';
    bool isC = ui->iscircle->isChecked();
    int func = ui->type->currentIndex();
    QString s = ui->input->toPlainText();
    if(s==""){
        QMessageBox::warning(this, "文本为空", "请输入一些单词");
        return;
    }
    char* word_buf = (char*)malloc(0x4000000);
    char** words = (char**)malloc(sizeof(char*) * MAX_WORD_NUM);
    int n_words;
    char *result_buf = (char *) malloc(0x10000000);
    char **result = (char**) malloc (sizeof (char*) * MAX_CHAIN_NUM);
    result[0] = result_buf;
    if (!result) {
        QMessageBox::warning(nullptr, "111", "111");
    }
    int n_return;
    HMODULE core = LoadLibraryA("D:\\CDLL\\cmake-build-debug\\libCDLL.dll");


    n_words = Qstring2words(s,word_buf, words);
    try{
        if (func == 0) {
            if(head!='\0'||tail!='\0'||isC){
                QMessageBox::warning(this, "参数选择有误", "-n不可与-h、-t、-r共同使用");
                return;
            }
            else{
                //-n
                n_return = gen_chains_all(words,n_words,(char**)(void*) result);
            }

        }else if(func == 1){
            // -w
            n_return = gen_chain_word(words, n_words, result, head, tail,forbid,isC);

        }else if(func == 2){
            // -c
            n_return = gen_chain_char(words, n_words, result, head, tail,forbid, isC);

        } else n_return = 0;
    }
    catch (std::exception &e) {
        QMessageBox::warning(this, "异常", e.what());
        return;
    }
    QString res;
    if(func == 0){
        res = QString::number(n_return);
        res.append('\n');
    }

    for (int i = 0; i < n_return; i++) {
        res.append(result[i]);
        res.append('\n');
    }
    ui->output->setPlainText(res);
    FreeLibrary(core);
    free(words);
    free(word_buf);
    free(result);
    free(result_buf);
}
void Widget::saveFile() {
    QString filename = QFileDialog::getSaveFileName(this,
            NULL,  "solution.txt", "Text files (*.txt)");
        if (filename.isEmpty()) {
            // User cancelled selecting file
            return;
        }
        QFile qFile(filename);
        if (!qFile.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this, "文件保存失败", "你选择的文件拒绝访问");
            return;
        }
        QTextStream stream(&qFile);
        stream << ui->output->toPlainText();
        qFile.close();

}
void Widget::on_loadFile_clicked()
{
    loadFile();
}


void Widget::on_save_clicked()
{
    saveFile();
}



void Widget::on_begin_clicked()
{

    myTimer->setInterval(100);
    myTimer->start(100);
    startCalc();
    myTimer->stop();

}

