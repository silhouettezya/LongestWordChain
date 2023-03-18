<script>
import { ref } from 'vue'
import axios from 'axios'
import { ElMessage } from 'element-plus'


export default{
  name: 'App',
  data: ()=>({
    head: '',
    tail: '',
    reject: '',
    input: '',
    output: '',
    calculating: false,
    timeout: 3000,
    w: false, c: false, n: false, r: false,
    h: false, t: false, j: false, 
    dialogVisible: false,
    command: "",
    start: true,
  }),
  methods:{
    handleChange(UploadFile, UploadFiles) {
      console.log("in handleChange.......");
      if(UploadFiles.length == 2){
        UploadFiles.shift()
      }
	    // 本地电脑路径
	    console.log(document.getElementsByClassName("el-upload__input")[0].value);
      console.log(UploadFile.name);
      let reader = new FileReader();
      reader.onload = async (e) => {
        //打印文件内容
        console.log(reader.result);
        this.input = reader.result;
        console.log(this.input);
      };
      reader.readAsText(UploadFile.raw);
      console.log("out handleChange.......");
    },
    changeInputEvent(val, item){
      item.searchText = val;
      this.$forceUpdate()
    },
    openError(string){
      this.start = false;
      ElMessage.error(string);
    },
    fake_click(obj) {
        var ev = document.createEvent("MouseEvents");
        ev.initMouseEvent(
            "click", true, false, window, 0, 0, 0, 0, 0
            , false, false, false, false, 0, null
        );
        obj.dispatchEvent(ev);
    },
    export_raw(name, data) {
        var urlObject = window.URL || window.webkitURL || window;
        var export_blob = new Blob([data]);
        var save_link = document.createElementNS("http://www.w3.org/1999/xhtml", "a");
        save_link.href = urlObject.createObjectURL(export_blob);
        save_link.download = name;
        this.fake_click(save_link);
    },
    writeOutput(){
      this.export_raw('output.txt', this.output);
    },
    getCommand(){
      if(this.n + this.w + this.c > 1){
        this.openError("只能选择一个计算方式！");
      }else{
        if(this.n){
          this.command = this.command + (' -n ');
        }
        if(this.w){
          this.command = this.command + (' -w ');
        }
        if(this.c){
          this.command = this.command + (' -c ');
        }
      }
      if(this.r){
        this.command = this.command + (' -r ');
      }
      if(this.h){
          if(this.head.length > 1){
            this.openError("首字母限制最多一个字符!");
          }else if(!(this.head>='a'&&this.head<='z' 
            || this.head>='A'&&this.head<='Z')){
              this.openError("首字母应当为合法字母！")
          }else{
            this.command = this.command + (' -h ' + head + ' ');
          }
      }
      if(this.t){
        if(this.tail.length > 1){
            this.openError("尾字母限制最多一个字符!");
          }else if(!(this.tail>='a'&&this.tail<='z' 
            || this.tail>='A'&&this.tail<='Z')){
              this.openError("尾字母应当为合法字母！")
          }else{
            this.command = this.command + (' -t ' + tail + ' ');
          }
      }
      if(this.j){
        if(this.reject.length > 1){
            this.openError("非首字母限制最多一个字符!");
          }else if(!(this.reject>='a'&&this.reject<='z' 
            || this.reject>='A'&&this.reject<='Z')){
              this.openError("非首字母应当为合法字母！")
          }else{
            this.command = this.command + (' -h ' + reject + ' ');
          }
      }
      this.command = this.command + (' input.txt');
    },
    // 读取文件
    readFile(filePath) {
        // 创建一个新的xhr对象
        let xhr = null
       if (window.XMLHttpRequest) {
          xhr = new XMLHttpRequest()
        } else {
          // eslint-disable-next-line
          xhr = new ActiveXObject('Microsoft.XMLHTTP')
        }
        const okStatus = document.location.protocol === 'file' ? 0 : 200
        xhr.open('GET', filePath, false)
       xhr.overrideMimeType('text/html;charset=utf-8')
       xhr.send(null)
       return xhr.status === okStatus ? xhr.responseText : null
     },
    submit(){
      this.getCommand();
      console.log(this.command);
      if(this.start) {
        this.dialogVisible = true;
        this.output = this.readFile('/bin/result.txt')
        console.log(this.output)
      }
      this.start = true;
    },
    solve() {
      this.calculating = true
      this.outputText = ''
      this.runMessage = ''
      let start = moment()
      core.async(
          this.inputText,
          [0, this.allowRing ? 3 : 1, 2, this.allowRing ? 3 : 1][this.selectedMode],
          this.noAvailableOptions || !this.head ? 0 : this.head.charCodeAt(0),
          this.noAvailableOptions || !this.tail ? 0 : this.tail.charCodeAt(0),
          this.selectedMode === 3,
          (e, d) => {
            if (e) this.reportError(e)
            if (/^WordList-GUI: /.test(d)) {
              this.reportError(d.substring(14))
            } else {
              this.outputText = d
              this.runMessage = '计算用时：' + moment().diff(start) + 'ms'
            }
            this.calculating = false
          }
      )
    },
  },
}

</script>

<template>
    <div class="common-layout">
    <el-container>
      <el-header>
        <span class="title">最长单词链计算</span>
      </el-header>
      <el-container>
        <el-aside width=350px>
          <div class="typeSelect">
          <p class="ins">求解类型：</p>
            <div class = "line">
              <p> 求所有单词链数目 </p>
              <div class="switch">
              <el-switch v-model="n" /></div>
            </div>
            <div class = "line">
              <p> 求最长单词单词链 </p>
              <div class="switch">
              <el-switch v-model="w" /></div>
            </div>
            <div class = "line">
              <p> 求最长字母单词链 </p>
              <div class="switch">
              <el-switch v-model="c" /></div>
            </div>
            <div class="fujia">
              <p class="ins"> 附加限制：</p>
              <div class = "line">
                <p> 允许出现环 </p>
                <div class="switch">
                <el-switch v-model="r" /></div>
              </div>
              <div>
                <div class = "line">
                  <p> 限定单词链首字母 </p>
                  <div class="switch">
                  <el-switch v-model="h" /></div>
                </div>
                <el-input v-model="head" :disabled = "!h" placeholder="head" />
                <div class = "line">
                  <p> 限定单词链尾字母 </p>
                  <div class="switch">
                  <el-switch v-model="t" /></div>
                </div>
                <el-input v-model="tail" :disabled = "!t" placeholder="tail" />
                <div class = "line">
                  <p> 拒绝单词首字母 </p>
                  <div class="switch">
                  <el-switch v-model="j" /></div>
                </div>
                <el-input v-model="reject" :disabled = "!j" placeholder="reject" />
              </div>
            </div>
          </div>
        </el-aside>
        <el-main>
        <div class="main">
          <p class="ins"> 请输入文本 </p>
          <div class="input">
          <el-input
            v-model="input"
            :autosize="{ minRows: 1, maxRows: 50 }"
            type="textarea"
            placeholder="Please input"
          />
          </div>
          <div class="upload">
          <el-upload
            class="upload-demo"
            action=""
            :limit="1"
            :on-change="handleChange"
            :auto-upload="false"
          >
            <template #trigger>
              <el-button type="primary">从文件中导入</el-button>
            </template>
            <template #tip>
              <div class="el-upload__tip text-red">
              </div>
            </template>
          </el-upload>
          <div class="reset"><el-button @click="
            n=false; w=false; c=false; r=false; h=false; t=false; j=false;
            head=''; tail=''; reject=''; input='';
          " plain>全局重置</el-button></div>
          </div>
          <el-button type="primary" plain @click="submit">
            <a href="Webshell://">确认</a>
          </el-button>
          <el-button type="primary" plain @click="submit">
            <a href="Write://">提交</a>
          </el-button>
        </div>
        </el-main>
      </el-container>
    </el-container>
    <el-dialog
      v-model="dialogVisible"
      title="结果"
      width="50%"
      :before-close="handleClose"
    >
     <span class="ins">结果如下：</span>
     <div class="result">
      <span>{{this.output}}</span>
      </div>
      <template #footer>
        <span class="dialog-footer">
          <el-button type="primary" @click="dialogVisible = false">
            确定
         </el-button>
         <el-button type="primary" @click="dialogVisible = false;this.writeOutput()">
            导出
         </el-button>
       </span>
      </template>
    </el-dialog>
  </div>
</template>

<style scoped>
/* header: */
el-header{
  align-content: center;
}
.title{
  padding-left: 45%;
  font-size: 25px;
  font-weight: bold;
  text-align: center;
}

/* aside: */
.typeSelect{
  border: 1px solid #ccc;
  border-radius: 16px;
  margin-top: 15%;
  margin-right: 5%;
  padding-block: 10%;
  padding-left: 10%;
  padding-right: 10%;
}
.ins{
  font-size: 20px;
  padding-bottom: 10px;
}
.fujia{
  margin-block: 10px;
}
.switch{
  margin-left: 40px;
  align-content: right;
  align-items: right;
}
el-switch{
  align-content: right;
  float: right;
}
.line{
  display: inline-flex;
  padding-block: 5px;
  width: 300px;
}

/* main: */
.main{
  padding-top: 20px;
  margin-left: 50px;
  width: 300px;
}
.upload{
  padding-block: 20px;
  display: inline-flex;
}
.reset{
  padding-left: 100px;
}
.result{
  margin-top: 15px;
  border: 1px solid #ccc;
  border-radius: 16px;
  padding-block: 10px;
  padding-left: 10px;
  padding-right: 10px;
}
</style>
