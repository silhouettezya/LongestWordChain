const express = require('express'); //npm下载并引入express模块 npm -express -D
const router = require('../router/index.js');
var ejs = require('ejs')
var path = require('path');


const app = express();
app.use('/plugIn',express.static(path.join(__dirname,'../plugIn'))) 

app.set('views', path.join(__dirname,'../views'));

app.set( 'view engine', 'html' );
app.engine( '.html', require( 'ejs' ).__express );

app.use(`/index`,router)

app.listen(8000,function(err){  //8080 想要监听项目的端口号
	if(err){
		console.log(err)
	}else {
		console.log('项目启动成功')
	}
})

const router = express.Router()


router.use((req, res, next) => {
  console.log(`路由执行成功啦~~~`, Date.now());
  next()
})

router.get(`/`, (req, res, next) => {
  res.render('org')
})

router.get(`/exec`, (req, res, next) => {
	require('child_process').exec('start E:\\StudyingFile\\Computer\\Software Engineering\\Pair Homework\\frontVue\\front\\1.exe')
	
	console.log(123)
  res.send('1')
})

module.exports = router