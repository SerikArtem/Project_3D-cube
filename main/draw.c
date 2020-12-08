#include "draw.h"
#include "string.h"

// Установка вокселя в ON
void setvoxel(int x, int y, int z)
{
	if (inrange(x,y,z))
	cube[z][y] |= (1 << x);
}

// Установка отдельного воксела во временной куб буфера
void tmpsetvoxel(int x, int y, int z)
{
	if (inrange(x,y,z))
	fb[z][y] |= (1 << x);
}

// Установка вокселя в OFF
void clrvoxel(int x, int y, int z)
{
	if (inrange(x,y,z))
	cube[z][y] &= ~(1 << x);
}

// Установка отдельного воксела в ON во временной куб буфера
void tmpclrvoxel(int x, int y, int z)
{
	if (inrange(x,y,z))
	fb[z][y] &= ~(1 << x);
}

// Проверка выхода за пределы куба
unsigned char inrange(int x, int y, int z)
{
	if (x >= 0 && x < CUBE_SIZE && y >= 0 && y < CUBE_SIZE && z >= 0 && z < CUBE_SIZE)
	{
		return 1;
	} else
	{
		
		return 0;
	}
}

// Пелучить текущее состояние вокселя
unsigned char getvoxel(int x, int y, int z)
{
	if (inrange(x,y,z))
	{
		if (cube[z][y] & (1 << x))
		{
			return 1;
		} else
		{
			return 0;
		}
	} else
	{
		return 0;
	}
}

// Запись вокселя по условию 
void altervoxel(int x, int y, int z, int state)
{
	if (state == 1)
	{
		setvoxel(x,y,z);
	} else
	{
		clrvoxel(x,y,z);
	}
}

// Инвентировать воксель
void flpvoxel(int x, int y, int z)
{
	if (inrange(x, y, z))
	cube[z][y] ^= (1 << x);
}

void setplane_z (int z)
{
	int i;
	if (z>=0 && z<CUBE_SIZE)
	{
		for (i=0;i<CUBE_SIZE;i++)
		cube[z][i] = 0xff;
	}
}

// Установка вокселей вдоль оси z
void clrplane_z (int z)
{
	int i;
	if (z>=0 && z<CUBE_SIZE)
	{
		for (i=0;i<CUBE_SIZE;i++)
		cube[z][i] = 0x00;
	}
}

void setplane_x (int x)
{
	int z;
	int y;
	if (x>=0 && x<CUBE_SIZE)
	{
		for (z=0;z<CUBE_SIZE;z++)
		{
			for (y=0;y<CUBE_SIZE;y++)
			{
				cube[z][y] |= (1 << x);
			}
		}
	}
}

void clrplane_x (int x)
{
	int z;
	int y;
	if (x>=0 && x<CUBE_SIZE)
	{
		for (z=0;z<CUBE_SIZE;z++)
		{
			for (y=0;y<CUBE_SIZE;y++)
			{
				cube[z][y] &= ~(1 << x);
			}
		}
	}
}

void setplane_y (int y)
{
	int z;
	if (y>=0 && y<CUBE_SIZE)
	{
		for (z=0;z<CUBE_SIZE;z++)
		cube[z][y] = 0xff;
	}
}

void clrplane_y (int y)
{
	int z;
	if (y>=0 && y<CUBE_SIZE)
	{
		for (z=0;z<CUBE_SIZE;z++)
		cube[z][y] = 0x00;
	}
}

void setplane (char axis, unsigned char i)
{
	switch (axis)
	{
		case AXIS_X:
		setplane_x(i);
		break;
		
		case AXIS_Y:
		setplane_y(i);
		break;

		case AXIS_Z:
		setplane_z(i);
		break;
	}
}

void clrplane (char axis, unsigned char i)
{
	switch (axis)
	{
		case AXIS_X:
		clrplane_x(i);
		break;
		
		case AXIS_Y:
		clrplane_y(i);
		break;

		case AXIS_Z:
		clrplane_z(i);
		break;
	}
}

// Заполнить куб значениями все 64 байта буфера куба
// В основном используется для полной очистки(0x00) или полного заполнения(0xff)
void fill (unsigned char pattern)
{
	int z;
	int y;
	for (z=0;z<CUBE_SIZE;z++)
	{
		for (y=0;y<CUBE_SIZE;y++)
		{
			cube[z][y] = pattern;
		}
	}
}

void tmpfill (unsigned char pattern)
{
	int z;
	int y;
	for (z=0;z<CUBE_SIZE;z++)
	{
		for (y=0;y<CUBE_SIZE;y++)
		{
			fb[z][y] = pattern;
		}
	}
}

// Прямоугольник все стены заполнены 
void box_filled(int x1, int y1, int z1, int x2, int y2, int z2)
{
	int iy;
	int iz;

	argorder(x1, x2, &x1, &x2);
	argorder(y1, y2, &y1, &y2);
	argorder(z1, z2, &z1, &z2);

	for (iz=z1;iz<=z2;iz++)
	{
		for (iy=y1;iy<=y2;iy++)
		{
			cube[iz][iy] |= byteline(x1,x2);
		}
	}

}

// Полая коробка с заполненными стенками 
void box_walls(int x1, int y1, int z1, int x2, int y2, int z2)
{
	int iy;
	int iz;
	
	argorder(x1, x2, &x1, &x2);
	argorder(y1, y2, &y1, &y2);
	argorder(z1, z2, &z1, &z2);

	for (iz=z1;iz<=z2;iz++)
	{
		for (iy=y1;iy<=y2;iy++)
		{
			if (iy == y1 || iy == y2 || iz == z1 || iz == z2)
			{
				cube[iz][iy] = byteline(x1,x2);
			} else
			{
				cube[iz][iy] |= ((0x01 << x1) | (0x01 << x2));
			}
		}
	}

}


// Рисует карказ коробки (стены пустые)
void box_wireframe(int x1, int y1, int z1, int x2, int y2, int z2)
{
	int iy;
	int iz;

	argorder(x1, x2, &x1, &x2);
	argorder(y1, y2, &y1, &y2);
	argorder(z1, z2, &z1, &z2);

	// Линии вдоль оси X
	cube[z1][y1] = byteline(x1,x2);
	cube[z1][y2] = byteline(x1,x2);
	cube[z2][y1] = byteline(x1,x2);
	cube[z2][y2] = byteline(x1,x2);

	//Линии вдоль оси Y
	for (iy=y1;iy<=y2;iy++)
	{
		setvoxel(x1,iy,z1);
		setvoxel(x1,iy,z2);
		setvoxel(x2,iy,z1);
		setvoxel(x2,iy,z2);
	}

	// Линии вдоль оси Z
	for (iz=z1;iz<=z2;iz++)
	{
		setvoxel(x1,y1,iz);
		setvoxel(x1,y2,iz);
		setvoxel(x2,y1,iz);
		setvoxel(x2,y2,iz);
	}

}


// Переворачивает байт 180 градусов
// Старший бит младшего разряда становится, становится старший бит младшего разряда
char flipbyte (char byte)
{
	char flop = 0x00;

	flop = (flop & 0b11111110) | (0b00000001 & (byte >> 7));
	flop = (flop & 0b11111101) | (0b00000010 & (byte >> 5));
	flop = (flop & 0b11111011) | (0b00000100 & (byte >> 3));
	flop = (flop & 0b11110111) | (0b00001000 & (byte >> 1));
	flop = (flop & 0b11101111) | (0b00010000 & (byte << 1));
	flop = (flop & 0b11011111) | (0b00100000 & (byte << 3));
	flop = (flop & 0b10111111) | (0b01000000 & (byte << 5));
	flop = (flop & 0b01111111) | (0b10000000 & (byte << 7));
	return flop;
}



// Калибровка задержки
void delay_ms(uint16_t x)
{
	uint8_t y, z;
	for ( ; x > 0 ; x--){
		for ( y = 0 ; y < 90 ; y++){
			for ( z = 0 ; z < 6 ; z++){
				asm volatile ("nop");
			}
		}
	}
}

// Копирование содержимого массива fb (временного буфера) в буфер исполнения
void tmp2cube (void)
{
	memcpy(cube, fb, 64);
}

// Сдвиг куба по оси
void shift (char axis, int direction)
{
	int i, x ,y;
	int ii, iii;
	int state;

	for (i = 0; i < CUBE_SIZE; i++)
	{
		if (direction == -1)
		{
			ii = i;
		} else
		{
			ii = (7-i);
		}
		
		
		for (x = 0; x < CUBE_SIZE; x++)
		{
			for (y = 0; y < CUBE_SIZE; y++)
			{
				if (direction == -1)
				{
					iii = ii+1;
				} else
				{
					iii = ii-1;
				}
				
				if (axis == AXIS_Z)
				{
					state = getvoxel(x,y,iii);
					altervoxel(x,y,ii,state);
				}
				
				if (axis == AXIS_Y)
				{
					state = getvoxel(x,iii,y);
					altervoxel(x,ii,y,state);
				}
				
				if (axis == AXIS_X)
				{
					state = getvoxel(iii,y,x);
					altervoxel(ii,y,x,state);
				}
			}
		}
	}
	
	if (direction == -1)
	{
		i = 7;
	} else
	{
		i = 0;
	}
	
	for (x = 0; x < CUBE_SIZE; x++)
	{
		for (y = 0; y < CUBE_SIZE; y++)
		{
			if (axis == AXIS_Z)
			clrvoxel(x,y,i);
			
			if (axis == AXIS_Y)
			clrvoxel(x,i,y);
			
			if (axis == AXIS_X)
			clrvoxel(i,y,x);
		}
	}
}

// Перевернуть куб на 180 градусов вдоль оси Y
void mirror_y (void)
{
	unsigned char buffer[CUBE_SIZE][CUBE_SIZE];
	unsigned char x,y,z;

	memcpy(buffer, cube, CUBE_BYTES); // Копировать текущий куб в буфер

	fill(0x00);
	for (z=0; z<CUBE_SIZE; z++)
	{
		for (y=0; y<CUBE_SIZE; y++)
		{
			for (x=0; x<CUBE_SIZE; x++)
			{
				if (buffer[z][y] & (0x01 << x))
				setvoxel(x,CUBE_SIZE-1-y,z);
			}
		}
	}

}

// Перевернуть куб на 180 градусов вдоль оси X
void mirror_x (void)
{
	unsigned char buffer[CUBE_SIZE][CUBE_SIZE];
	unsigned char y,z;

	memcpy(buffer, cube, CUBE_BYTES); // Копировать текущий куб в буфер

	fill(0x00);
	
	for (z=0; z<CUBE_SIZE; z++)
	{
		for (y=0; y<CUBE_SIZE; y++)
		{
			cube[z][y] = flipbyte(buffer[z][y]);
		}
	}
}

// Перевернуть куб на 180 градусов по оси Z
void mirror_z (void)
{
	unsigned char buffer[CUBE_SIZE][CUBE_SIZE];
	unsigned char z, y;

	memcpy(buffer, cube, CUBE_BYTES); // Копировать текущий куб в буфер 

	for (y=0; y<CUBE_SIZE; y++)
	{
		for (z=0; z<CUBE_SIZE; z++)
		{
			cube[CUBE_SIZE-1-z][y] = buffer[z][y];
		}
	}
}

