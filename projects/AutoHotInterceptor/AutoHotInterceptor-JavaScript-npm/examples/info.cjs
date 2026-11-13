const {Interceptor}=require("autohotinterceptor");const io=new Interceptor();try{console.log(io.info())}finally{io.close()}
