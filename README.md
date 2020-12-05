# pi_ntc_cpp

use wiringpi, default spi0, no access to spi1

# ntc.cc
use arbitrary GPIO to select chip, need to add 'dtoverlay=spi0-hw-cs' in "/boot/config.txt"

# ntc_sc.cc
use GPIO7,8 (SPI0_CE0,1) to select chip.

