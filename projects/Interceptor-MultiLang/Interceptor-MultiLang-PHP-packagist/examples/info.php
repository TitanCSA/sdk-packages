<?php
$paths = [dirname(__DIR__).'/vendor/autoload.php', dirname(__DIR__,3).'/autoload.php'];
$loaded=false; foreach($paths as $path) { if(is_file($path)) { require $path; $loaded=true; break; } }
if(!$loaded) require dirname(__DIR__).'/src/Interceptor.php';
$io = new TitanCSA\Interceptor\Interceptor();
try { print_r($io->info()); } finally { $io->close(); }
