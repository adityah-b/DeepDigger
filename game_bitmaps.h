#include <lpc17xx.h>
#include <stdio.h>
#include <stdbool.h>
#include <RTL.h>

uint8_t digger_bmp[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x96, 0xb5, 0x71, 0x8c, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0xb2, 0x94, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x69, 0x4a, 0x41, 0x08, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0x41, 0x08, 0xbe, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3c, 0xe7, 0x20, 0x00, 0x24, 0x21, 0x82, 0x10, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0xc3, 0x18, 0xc3, 0x18, 0x4d, 0x6b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xba, 0xd6, 0x82, 0x10, 0x82, 0x10, 0x5d, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x2c, 0x63, 0xc3, 0x18, 0x41, 0x08, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3c, 0xe7, 0xb6, 0xb5, 0x3c, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xba, 0xd6, 0x82, 0x10, 0x82, 0x10, 0xba, 0xd6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3c, 0xe7, 0x41, 0x08, 0xa2, 0x10, 0x51, 0x8c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb2, 0x94, 0x20, 0x00, 0x82, 0x10, 0x61, 0x08, 0x82, 0x10, 0x86, 0x31, 0xeb, 0x5a, 0x51, 0x8c, 0xd7, 0xbd, 0x3c, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xba, 0xd6, 0x82, 0x10, 0x82, 0x10, 0xba, 0xd6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x28, 0x42, 0xe3, 0x18, 0xa2, 0x10, 0xff, 0xff, 0xff, 0xff, 0xcb, 0x5a, 0x61, 0x08, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x04, 0x21, 0xe3, 0x18, 0xa2, 0x10, 0x82, 0x10, 0x61, 0x08, 0x82, 0x10, 0x7d, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xba, 0xd6, 0x82, 0x10, 0x82, 0x10, 0xba, 0xd6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x38, 0xc6, 0x61, 0x08, 0x82, 0x10, 0x96, 0xb5, 0x1c, 0xe7, 0x00, 0x00, 0x24, 0x21, 0x24, 0x21, 0xe3, 0x18, 0xc3, 0x18, 0xe3, 0x18, 0x04, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0xa2, 0x10, 0x8e, 0x73, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xba, 0xd6, 0x82, 0x10, 0x82, 0x10, 0xba, 0xd6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x45, 0x29, 0xe3, 0x18, 0x45, 0x29, 0xff, 0xff, 0x08, 0x42, 0xe3, 0x18, 0x61, 0x08, 0xeb, 0x5a, 0xef, 0x7b, 0x28, 0x42, 0xc3, 0x18, 0x41, 0x08, 0x82, 0x10, 0x24, 0x21, 0x24, 0x21, 0xc3, 0x18, 0x6d, 0x6b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xba, 0xd6, 0x82, 0x10, 0x82, 0x10, 0xfb, 0xde, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x34, 0xa5, 0x82, 0x10, 0x61, 0x08, 0x9a, 0xd6, 0x79, 0xce, 0x00, 0x00, 0xb2, 0x94, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x59, 0xce, 0x82, 0x10, 0x24, 0x21, 0xc3, 0x18, 0x8e, 0x73, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xba, 0xd6, 0x61, 0x08, 0xc3, 0x18, 0x71, 0x8c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc3, 0x18, 0xe3, 0x18, 0x49, 0x4a, 0xff, 0xff, 0x38, 0xc6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x61, 0x08, 0x24, 0x21, 0xc3, 0x18, 0x8e, 0x73, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x79, 0xe7, 0x15, 0xd7, 0xf5, 0xce, 0xf5, 0xce, 0x58, 0xdf, 0xff, 0xff, 0x61, 0x08, 0x04, 0x21, 0x61, 0x08, 0xae, 0x73, 0xb2, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0xf3, 0x9c, 0x69, 0x4a, 0x04, 0x21, 0x41, 0x08, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0x61, 0x08, 0x24, 0x21, 0xc3, 0x18, 0x8e, 0x73, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xea, 0x9d, 0xc8, 0x95, 0xc9, 0x9d, 0xe9, 0x9d, 0xc9, 0x9d, 0xa8, 0x95, 0xff, 0xff, 0xdb, 0xde, 0x41, 0x08, 0x04, 0x21, 0xc3, 0x18, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0x04, 0x21, 0x24, 0x21, 0x20, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0x61, 0x08, 0x24, 0x21, 0xc3, 0x18, 0x8e, 0x73, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0x70, 0xbe, 0xe9, 0x9d, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0xea, 0x9d, 0xea, 0xa5, 0xff, 0xff, 0xbb, 0xde, 0x41, 0x08, 0x04, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x41, 0x08, 0xae, 0x73, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x61, 0x08, 0x24, 0x21, 0xe3, 0x18, 0x2c, 0x63, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0x4e, 0xb6, 0xea, 0x9d, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0xea, 0x9d, 0xea, 0x9d, 0xff, 0xff, 0xdb, 0xde, 0x41, 0x08, 0x04, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x04, 0x21, 0x41, 0x08, 0x34, 0xa5, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xba, 0xd6, 0x82, 0x10, 0x24, 0x21, 0x24, 0x21, 0xa2, 0x10, 0x69, 0x4a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0x4e, 0xb6, 0xea, 0x9d, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0xea, 0x9d, 0xea, 0x9d, 0xff, 0xff, 0xbb, 0xde, 0x20, 0x00, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0x82, 0x10, 0x04, 0x21, 0x7e, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x28, 0x42, 0xe3, 0x18, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0xa2, 0x10, 0x08, 0x42, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0x4e, 0xb6, 0xea, 0x9d, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0xea, 0x9d, 0xea, 0x9d, 0xff, 0xff, 0xbf, 0xff, 0xb3, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x35, 0xa5, 0xff, 0xff, 0xdc, 0xf7, 0xf4, 0xce, 0xf4, 0xce, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdb, 0xde, 0x41, 0x08, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x61, 0x08, 0xb6, 0xb5, 0xff, 0xff,
  0xff, 0xff, 0x4e, 0xb6, 0xea, 0x9d, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0xea, 0x9d, 0xe9, 0x9d, 0x36, 0xdf, 0xfd, 0xff, 0xfd, 0xff, 0xfd, 0xff, 0xfd, 0xff, 0xdc, 0xf7, 0xd2, 0xc6, 0xc8, 0x95, 0xc9, 0x9d, 0xc8, 0x95, 0x6f, 0xb6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x08, 0x42, 0xe3, 0x18, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0xe3, 0x18, 0xcb, 0x5a, 0xff, 0xff,
  0xff, 0xff, 0x4e, 0xb6, 0xea, 0x9d, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0x0a, 0xa6, 0xea, 0xa5, 0xc9, 0x9d, 0xc8, 0x9d, 0xc8, 0x9d, 0xc8, 0x9d, 0xc8, 0x9d, 0xc9, 0x9d, 0xe9, 0x9d, 0xea, 0xa5, 0x0a, 0xa6, 0xea, 0x9d, 0x4e, 0xb6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdb, 0xde, 0x41, 0x08, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0xe3, 0x18, 0x69, 0x4a, 0xff, 0xff,
  0xff, 0xff, 0xb1, 0xbe, 0xa7, 0x95, 0xc8, 0x9d, 0xc8, 0x9d, 0xc8, 0x9d, 0xc8, 0x9d, 0xc8, 0x9d, 0xc8, 0x9d, 0xc8, 0x9d, 0xc8, 0x9d, 0xc8, 0x9d, 0xc8, 0x9d, 0xc8, 0x9d, 0xc8, 0x9d, 0xc8, 0x9d, 0xc8, 0x9d, 0xc8, 0x9d, 0xc8, 0x9d, 0xc8, 0x9d, 0xc8, 0x9d, 0xc8, 0x9d, 0xa7, 0x95, 0xb1, 0xbe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x08, 0x42, 0xe3, 0x18, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0xc3, 0x18, 0xcf, 0x7b, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x9b, 0xef, 0x9b, 0xef, 0xdc, 0xf7, 0xfd, 0xff, 0xfd, 0xff, 0xfd, 0xff, 0xfd, 0xff, 0xfd, 0xff, 0xfd, 0xff, 0xfd, 0xff, 0xfd, 0xff, 0xfd, 0xff, 0xfd, 0xff, 0xfd, 0xff, 0xfd, 0xff, 0xfd, 0xff, 0xfd, 0xff, 0xfd, 0xff, 0xfd, 0xff, 0xdc, 0xf7, 0x9b, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x79, 0xce, 0x61, 0x08, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x20, 0x00, 0x5d, 0xef, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7e, 0xef, 0x14, 0xa5, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0xd3, 0x9c, 0x9a, 0xd6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd7, 0xbd, 0xa6, 0x31, 0x82, 0x10, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x61, 0x08, 0x30, 0x84, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xeb, 0x5a, 0x20, 0x00, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0x41, 0x08, 0x86, 0x31, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa2, 0x10, 0xc3, 0x18, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x24, 0x21, 0x04, 0x21, 0xe3, 0x18, 0x20, 0x00, 0xcf, 0x7b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x49, 0x4a, 0xa2, 0x10, 0xe3, 0x18, 0x61, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0xc3, 0x18, 0xe3, 0x18, 0xc3, 0x18, 0xff, 0xff, 0xff, 0xff, 0x71, 0x8c, 0x00, 0x00, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0xa2, 0x10, 0x8a, 0x52, 0xba, 0xd6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0x18, 0xc6, 0x41, 0x08, 0xc3, 0x18, 0x8a, 0x52, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x51, 0x8c, 0x82, 0x10, 0xa2, 0x10, 0xcf, 0x7b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0x2c, 0x63, 0xc3, 0x18, 0xe3, 0x18, 0xff, 0xff, 0x92, 0x94, 0x49, 0x4a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x4d, 0x6b, 0x4d, 0x6b, 0xff, 0xff, 0xaa, 0x52, 0xc3, 0x18, 0x04, 0x21, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0x49, 0x4a, 0xa2, 0x10, 0x8a, 0x52, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xd3, 0x9c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xba, 0xd6, 0x00, 0x00, 0x00, 0x00, 0x1c, 0xe7, 0xf3, 0x9c, 0xa2, 0x10, 0x20, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0x2c, 0x63, 0xc3, 0x18, 0xe3, 0x18, 0xff, 0xff, 0xd3, 0x9c, 0x8a, 0x52, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x4d, 0x6b, 0x4d, 0x6b, 0xff, 0xff, 0xaa, 0x52, 0xc3, 0x18, 0x04, 0x21, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0x18, 0xc6, 0x41, 0x08, 0xc3, 0x18, 0x8a, 0x52, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x51, 0x8c, 0x82, 0x10, 0xa2, 0x10, 0xcf, 0x7b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x49, 0x4a, 0xa2, 0x10, 0xe3, 0x18, 0x61, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0xc3, 0x18, 0xe3, 0x18, 0xc3, 0x18, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xcb, 0x5a, 0x20, 0x00, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0xa2, 0x10, 0x41, 0x08, 0x86, 0x31, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x5d, 0xef, 0xf3, 0x9c, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0x92, 0x94, 0xb2, 0x94, 0x9a, 0xd6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,

};
