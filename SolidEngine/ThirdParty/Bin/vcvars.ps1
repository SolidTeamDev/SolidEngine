$path = D:\Project\SolidEngine\Build\Debug\Testing\MyProj\Code\Dlls\vswhere.exe -latest -find VC\Auxiliary\Build\vcvarsall.bat | select-object -first 1 
if ($path) { echo $path }