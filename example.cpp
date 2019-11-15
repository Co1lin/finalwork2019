#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// AAAAAAAARRRRRRRRGGGGGGGGBBBBBBBB
// 76543210765432107654321076543210
#define BMP_RGBA32(r,g,b,a)  (U4)( ((U4)(U1)(r)<<16) | ((U4)(U1)(g)<<8) | (U4)(U1)(b) | ((U4)(U1)(a)<<24) )
#define BMP_RGB24(r,g,b)     (U4)( ((U4)(U1)(r)<<16) | ((U4)(U1)(g)<<8) | (U4)(U1)(b) )

// XRRRRRGGGGGBBBBB
// 0432104321043210
#define BMP_RGBA32TOBMP16(c) (U2)( (((U4)(c)>>9) & 0x7C00u) | (((U4)(c)>>6) & 0x03E0u) | (((U4)(c)>>3) & 0x001F) )

typedef unsigned char  U1;
typedef unsigned short U2;
typedef unsigned long  U4;

typedef unsigned char  BOOL;
#define TRUE  (0xFFu)
#define FALSE (0x00u)

void vd_SerializeLittleEndianU2(U1 * u1_ap_serial, U2 u2_a_value)
{
	do
	{
		if (u1_ap_serial == NULL)
			break;

		u1_ap_serial[0] = (U1)u2_a_value;
		u1_ap_serial[1] = (U1)(u2_a_value >> 8);
	} while (FALSE);
}

void vd_SerializeLittleEndianU3(U1 * u1_ap_serial, U4 u4_a_value)
{
	do
	{
		if (u1_ap_serial == NULL)
			break;

		u1_ap_serial[0] = (U1)u4_a_value;
		u1_ap_serial[1] = (U1)(u4_a_value >> 8);
		u1_ap_serial[2] = (U1)(u4_a_value >> 16);
	} while (FALSE);
}

void vd_SerializeLittleEndianU4(U1 * u1_ap_serial, U4 u4_a_value)
{
	do
	{
		if (u1_ap_serial == NULL)
			break;

		u1_ap_serial[0] = (U1)u4_a_value;
		u1_ap_serial[1] = (U1)(u4_a_value >> 8);
		u1_ap_serial[2] = (U1)(u4_a_value >> 16);
		u1_ap_serial[3] = (U1)(u4_a_value >> 24);
	} while (FALSE);
}
typedef struct {
	U4 u4_image_width;
	U4 u4_image_height;
	U4 u4_widthbyte;
	U4 u4_image_size;
	U2 u2_bitcount;
	U2 u2_palette_size;
	U1* u1_p_image_data;
} ST_BITMAP;

ST_BITMAP * st_g_CreateBitmap(U4 u4_a_width, U4 u4_a_height, U2 u2_a_bitcount)
{
	BOOL b_t_success = FALSE;
	ST_BITMAP * st_tp_bitmap = NULL;
	U4 u4_t_widthbyte = 0;
	U4 u4_t_imagesize = 0;

	do
	{
		if ((u4_a_width == 0) || (u4_a_width > 4096))
			break;

		if ((u4_a_height == 0) || (u4_a_height > 4096))
			break;

		if ((u2_a_bitcount != 16) && (u2_a_bitcount != 24) && (u2_a_bitcount != 32))
			break;

		// 4-byte aligned bytes for a line of image data
		u4_t_widthbyte = (u4_a_width * u2_a_bitcount + 31) / 32 * 4;
		u4_t_imagesize = (u4_t_widthbyte * u4_a_height);

		st_tp_bitmap = (ST_BITMAP*)malloc(sizeof(ST_BITMAP) + u4_t_imagesize); // alloc together
		if (st_tp_bitmap == NULL)
			break;

		memset(st_tp_bitmap, 0, sizeof(ST_BITMAP) + u4_t_imagesize);

		st_tp_bitmap->u4_image_width = u4_a_width;
		st_tp_bitmap->u4_image_height = u4_a_height;
		st_tp_bitmap->u4_widthbyte = u4_t_widthbyte;
		st_tp_bitmap->u4_image_size = u4_t_imagesize;
		st_tp_bitmap->u2_bitcount = u2_a_bitcount;
		st_tp_bitmap->u2_palette_size = 0;
		// pointer to the address next to the struct
		st_tp_bitmap->u1_p_image_data = (U1 *)st_tp_bitmap + sizeof(ST_BITMAP);

		b_t_success = TRUE;
	} while (FALSE);

	return st_tp_bitmap;
}
void vd_g_FreeBitmap(ST_BITMAP * st_ap_bitmap)
{
	BOOL b_t_success = FALSE;

	do
	{
		if (st_ap_bitmap == NULL)
			break;

		memset(st_ap_bitmap, 0, sizeof(ST_BITMAP));
		free(st_ap_bitmap);

		b_t_success = TRUE;
	} while (FALSE);
}
void vd_g_SaveBitmap(const ST_BITMAP * st_ap_bitmap, const char * sz_ap_path)
{
	BOOL b_t_success = FALSE;

	U1 u1_tp_bitmap_header[] =
	{
		0x42, 0x4D, 0xAA, 0xAA, 0xAA, 0xAA, 0x00, 0x00, //  2 AA->FileSize
		0x00, 0x00, 0xBB, 0xBB, 0xBB, 0xBB, 0x28, 0x00, // 10 BB->OffBits
		0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xDD, 0xDD, // 18 CC->Width
		0xDD, 0xDD, 0x01, 0x00, 0xEE, 0xEE, 0x00, 0x00, // 22 DD->Height
		0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, // 28 EE->BitCount
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 34 FF->ImageSize
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	FILE *file_bitmap = NULL;

	U4 u4_t_y;
	U4 u4_t_pixel_offset = 0;

	do
	{
		if (st_ap_bitmap == NULL)
			break;

		if ((st_ap_bitmap->u2_bitcount != 16) && (st_ap_bitmap->u2_bitcount != 24) && (st_ap_bitmap->u2_bitcount != 32))
			break;

		if (sz_ap_path == NULL)
			break;

		file_bitmap = fopen(sz_ap_path, "wb");
		if (file_bitmap == NULL)
			break;

		// set bitmap head info
		vd_SerializeLittleEndianU4(&u1_tp_bitmap_header[2], sizeof(u1_tp_bitmap_header) + st_ap_bitmap->u4_image_size);
		vd_SerializeLittleEndianU4(&u1_tp_bitmap_header[10], sizeof(u1_tp_bitmap_header));
		vd_SerializeLittleEndianU4(&u1_tp_bitmap_header[18], st_ap_bitmap->u4_image_width);
		vd_SerializeLittleEndianU4(&u1_tp_bitmap_header[22], st_ap_bitmap->u4_image_height);
		vd_SerializeLittleEndianU2(&u1_tp_bitmap_header[28], st_ap_bitmap->u2_bitcount);
		vd_SerializeLittleEndianU4(&u1_tp_bitmap_header[34], st_ap_bitmap->u4_image_size);

		// write bitmap file head
		fwrite(u1_tp_bitmap_header, sizeof(u1_tp_bitmap_header), 1L, file_bitmap);

		// write bitmap image data, bottom to top
		u4_t_pixel_offset = st_ap_bitmap->u4_image_height * st_ap_bitmap->u4_widthbyte;
		for (u4_t_y = 0; u4_t_y < st_ap_bitmap->u4_image_height; u4_t_y++)
		{
			u4_t_pixel_offset -= st_ap_bitmap->u4_widthbyte;
			fwrite(&st_ap_bitmap->u1_p_image_data[u4_t_pixel_offset], st_ap_bitmap->u4_widthbyte, 1L, file_bitmap);
		}

		b_t_success = TRUE;

	} while (0);

	if (file_bitmap)
		fclose(file_bitmap);
}
void vd_SetBitmapPixel(ST_BITMAP * st_ap_bitmap, U4 u4_a_x, U4 u4_a_y, U4 u4_a_color)
{
	U4 u4_t_pixel_offset = 0;
	U2 u2_t_color = 0;

	do
	{

		if (st_ap_bitmap == NULL)
			break;

		if (u4_a_x >= st_ap_bitmap->u4_image_width)
			break;

		if (u4_a_y >= st_ap_bitmap->u4_image_height)
			break;

		u4_t_pixel_offset = u4_a_y * st_ap_bitmap->u4_widthbyte + u4_a_x * st_ap_bitmap->u2_bitcount / 8;

		switch (st_ap_bitmap->u2_bitcount)
		{
		case 16:
			u2_t_color = BMP_RGBA32TOBMP16(u4_a_color);
			vd_SerializeLittleEndianU2(&st_ap_bitmap->u1_p_image_data[u4_t_pixel_offset], u2_t_color);
			break;
		case 24:
			vd_SerializeLittleEndianU3(&st_ap_bitmap->u1_p_image_data[u4_t_pixel_offset], u4_a_color);
			break;
		case 32:
			vd_SerializeLittleEndianU4(&st_ap_bitmap->u1_p_image_data[u4_t_pixel_offset], u4_a_color);
			break;
		default:
			break;
		}
	} while (FALSE);
}
int main()
{
	ST_BITMAP * st_tp_bitmap = NULL;

	do
	{
		// create bitmap, size:20x10 format:RGB555->16
		// also support format RGB888->24 and RGBA8888->32
		st_tp_bitmap = st_g_CreateBitmap(20, 10, 16);
		if (st_tp_bitmap == NULL)
			break;

		// draw pixels on bitmap
		vd_SetBitmapPixel(st_tp_bitmap, 0, 0, BMP_RGB24(255, 0, 0)); // red
		vd_SetBitmapPixel(st_tp_bitmap, 1, 1, BMP_RGB24(0, 255, 0)); // green
		vd_SetBitmapPixel(st_tp_bitmap, 2, 2, BMP_RGB24(0, 0, 255)); // blue

		// save to file
		vd_g_SaveBitmap(st_tp_bitmap, "test.bmp");

	} while (FALSE);

	if (st_tp_bitmap)
		vd_g_FreeBitmap(st_tp_bitmap);

	return 0;
}
