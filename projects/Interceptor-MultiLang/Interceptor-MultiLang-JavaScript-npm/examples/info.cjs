const { Interceptor } = require('interceptor-multilang');
const io = new Interceptor();
try { console.log(io.info()); } finally { io.close(); }
