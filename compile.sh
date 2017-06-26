
echo "Defining gcc path..."
export PATH="$PATH:/Users/marcosgomes/git/smoothie-v2/gcc-arm-none-eabi_6/bin"

echo "Nuttx compile start..."
cd smoothie-nuttx/nuttx/tools/
./configure.sh bambino-200e/smoothiedev
cd ..
mv nuttx-export.zip nuttx-export.zip.bck
make export

unzip nuttx-export.zip

echo "Extracting nuutx-export to Firmware folder."
cd ../../Firmware/
rm -R nuttx-export/
mv ../smoothie-nuttx/nuttx/nuttx-export nuttx-export/

echo "Extracting nuutx-export to Dev folder."
cd ../Dev/
rm -R nuttx-export/
cp -R ../Firmware/nuttx-export/ nuttx-export/

cd ..
echo "Finish..."