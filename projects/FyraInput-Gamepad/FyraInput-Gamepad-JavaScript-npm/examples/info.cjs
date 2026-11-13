const {Interceptor}=require("fyrainput-gamepad");const io=new Interceptor();try{console.log(io.info())}finally{io.close()}
