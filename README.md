# themizer

themize reduces an image to a specific color palette.

## How to install
```
git clone https://github.com/MrBaduuz/themizer.git
cd themizer
sudo make install
```

## CMD options
* -i: Specify path to the input image
* -o: Specify path to the output image
* -p: Specify path to the palette file
* ( -f: Format of the output image (currently supported: png,jpg,bmp) )
* ( -d: Distance function (either: linear or squared) )
* ( -dt: Ditter the image )
* ( -rd: Reduce the color space before applying the palette )
* ( -b[rgb]: Brightness multiplier for RGB channels that is applied before applying the palette )

## Example
credit: https://whvn.cc/m9w1ym
![Example image](example_image.png "Example image")
