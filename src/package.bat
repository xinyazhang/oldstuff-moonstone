cp Debug/gui.exe package/
cp Debug/kernel.dll package/
cd package
7z a "-xr!.svn" unnamed.7z *
cd ..
mv package/unnamed.7z .
