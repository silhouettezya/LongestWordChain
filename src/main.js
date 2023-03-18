import { createApp } from 'vue'
import ElementPlus from 'element-plus'
import 'element-plus/dist/index.css'
import App from './App.vue'
import axios from 'axios'

import './assets/main.css'

const app =createApp(App)

app.config.globalProperties.$https = axios

app.use(ElementPlus)
app.mount('#app')

