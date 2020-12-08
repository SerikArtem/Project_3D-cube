#include "effect.h"
#include "draw.h"
#include "font.h"
#include <math.h>
#include <avr/interrupt.h>

void effect_test (void)
{

	int x,y,i;

	for (i=0;i<1000;i++)
	{
		x = sin(i/8)*2+3.5;
		y = cos(i/8)*2+3.5;

		setvoxel(x,y,1);
		setvoxel(x,y,1);
		delay_ms(1000);
		fill(0x00);
	}

}

void TEST_effect (unsigned char pattern)
{
	
	int z;
	int i;
	int y;
	int yy;
	for(i=0; i<10000; i++){
		
	
		for(z=0; z<8; z++){
		
		
		
			for(y=0; y<8; y++){	
				cube[z][y]=0xFF;
			}
		
			delay_ms(10000);
		
			for(yy=0; yy<8; yy++){
				cube[z][yy]=0x00;
			}
		
		
		}
		
	//-----------------------------------------
		//for (z=0; z<8; z++)
		//{
			//for (y=0; y<8; y++)
			//{
				//cube[z][y] = pattern;
			//}
		//}
		//for (ii = 0; ii<7; ii++)
		//{
			//delay_ms(2000);
			//shift(AXIS_Y,1);
		//}
		
	//-----------------------------------------
		
	}
	
	//int i, ii;
	//for(ii=0; ii<1000; ii++){
		//for(i=0; i<100; i++){
			//setvoxel(0,0,0); //0
			//setvoxel(0,0,7); //1
			//setvoxel(0,7,0); //2
			//setvoxel(0,7,7); //3
		//
			//setvoxel(7,0,0); //4
			//setvoxel(7,0,7); //5
			//setvoxel(7,7,0); //6
			//setvoxel(7,7,7); //7
		//}
		//delay_ms(2000);
		//shift(AXIS_Z,-1);
		//for(i=0; i<100; i++){
			//clrvoxel(0,0,0); //0
			//clrvoxel(0,0,7); //1
			//clrvoxel(0,7,0); //2
			//clrvoxel(0,7,7); //3
			//
			//clrvoxel(7,0,0); //4
			//clrvoxel(7,0,7); //5
			//clrvoxel(7,7,0); //6
			//clrvoxel(7,7,7); //7
		//}
		//delay_ms(2000);
		//shift(AXIS_Z,-1);
	//}
}

// 1) Эфект бегущей строки
void effect_stringfly2(char * str)
{
	int x,y,i;
	unsigned char chr[7];
	
	while (*str)
	{
		//font_getchar(*str++, chr);
		font_getchar_rus(*str++, chr);
		
		// Разместить символ на обратной стороне куба
		for (x = 0; x < 7; x++)
		{
			for (y = 0; y < 8; y++)
			{
				if ((chr[x] & (0x80>>y)))
				{
					setvoxel(x+1,0,y);
				}
			}
		}
		
		// Сдвигаем все содержимое куба вперед на 6 шагов 
		// перед размещением следующего символа
		for (i = 0; i<7; i++)
		{
			delay_ms(2000);
			shift(AXIS_Y,1);
		}
	}
	// Заменить последний символ к кубе
	for (i = 0; i<8; i++)
	{
		delay_ms(1000);
		shift(AXIS_Y,1);
	}
}


// 2) Слои и боковые грани быстро передвигаются 
void effect_planboing (int plane, int speed)
{
	int i;
	for (i=0;i<8;i++)
	{
		fill(0x00);
		setplane(plane, i);
		delay_ms(speed);
	}
	
	for (i=7;i>=0;i--)
	{
		fill(0x00);
		setplane(plane,i);
		delay_ms(speed);
	}
}

// 3) Эффект блика
void effect_blinky2()
{
	int i,r;
	fill(0x00);
	
	for (r=0;r<2;r++)
	{
		i = 750;
		while (i>0)
		{
			fill(0x00);
			delay_ms(i);
			
			fill(0xff);
			delay_ms(100);
			
			i = i - (15+(1000/(i/10)));
		}
		
		delay_ms(1000);
		
		i = 750;
		while (i>0)
		{
			fill(0x00);
			delay_ms(751-i);
			
			fill(0xff);
			delay_ms(100);
			
			i = i - (15+(1000/(i/10)));
		}
	}

}

// 4) Квадраты передвигаются из угла в угол, сужаются, расширяются
void effect_box_shrink_grow (int iterations, int rot, int flip, uint16_t delay)
{
	int x, i, xyz;
	for (x=0;x<iterations;x++)
	{
		for (i=0;i<16;i++)
		{
			xyz = 7-i;
			if (i > 7)
			xyz = i-8;
			
			fill(0x00); delay_ms(1);
			cli(); // Запрет прерывания, куб продолжает отрабатывать
			box_wireframe(0,0,0,xyz,xyz,xyz); // Функция рисует карказ коробки куба (без стенок)

			if (flip > 0)
			mirror_z(); // Поворат куба на 180 градусов относитено оси Z

			if (rot == 1 || rot == 3)
			mirror_y(); // Поворат куба на 180 градусов относитено оси Y

			if (rot == 2 || rot == 3)
			mirror_x(); // Поворат куба на 180 градусов относитено оси X

			sei(); // Разрешает прерывание
			delay_ms(delay);
			fill(0x00); // Очистка куба
		}
	}
}


// 5) Квадрат расширяется от середины
void effect_box_woopwoop (int delay, int grow)
{
	int i,ii;
	
	fill(0x00); // Очистка куба
	for (i=0;i<4;i++) // Черыре периода роста и сужения квадрата
	{
		ii = i; 
		if (grow > 0)
		ii = 3-i;

		box_wireframe(4+ii,4+ii,4+ii,3-ii,3-ii,3-ii); // Функция рисует каркас коробки куба (без стенок)
		delay_ms(delay); // Задержка 
		fill(0x00); // Очистка куба
	}
}


// Отправляет воксель, летящий с одной стороны куба на другую. Если он внизу, отправьте его наверх.
void sendvoxel_z (unsigned char x, unsigned char y, unsigned char z, int delay)
{
	int i, ii;
	for (i=0; i<8; i++)
	{
		if (z == 7)
		{
			ii = 7-i;
			clrvoxel(x,y,ii+1);
		} else
		{
			ii = i;
			clrvoxel(x,y,ii-1);
		}
		setvoxel(x,y,ii);
		delay_ms(delay);
	}
}

// Отправляем все воксели с одной стороны куба на другую.
// Начинаем с Z и отправляем на противоположную сторону.
// Отправляет в случайном порядке.
void sendplane_rand_z (unsigned char z, int delay, int wait)
{
	unsigned char loop = 16;
	unsigned char x, y;

	fill(0x00);

	setplane_z(z);
	
	// Отправляйте воксели рандомно, пока все 16 не пересекут куб.
	while(loop)
	{
		x = rand()%4;
		y = rand()%4;
		if (getvoxel(x,y,z))
		{
			// Отправить воксели в полёт
			sendvoxel_z(x,y,z,delay);
			delay_ms(wait);
			loop--;
		}
	}
}

// 6) По одному воксилю перелетает сверху вниз и наоборот 
void sendvoxels_rand_z (int iterations, int delay, int wait)
{
	unsigned char x, y, last_x = 0, last_y = 0, i;

	fill(0x00);

	// Перебирает все X и Y координаты
	for (x=0;x<8;x++)
	{
		for (y=0;y<8;y++)
		{
			//Установка вокселей вверху и внизу в случайном порядке
			setvoxel(x,y,((rand()%2)*7));
		}
	}

	for (i=0;i<iterations;i++)
	{
		// Выбирает все случайные xy координаты
		x = rand()%8;
		y = rand()%8;
		if (y != last_y && x != last_x)
		{
			// Если воксель находится внизу 
			if (getvoxel(x,y,0))
			{
				// Отправка вокселя наверх 
				sendvoxel_z(x,y,0,delay);
			} else
			{
				// Если воксель сверх, то отправить его вниз
				sendvoxel_z(x,y,7,delay);
			}
			delay_ms(wait);
			
			// Запоминает последний ход 
			last_y = y;
			last_x = x;
		}
	}

}

// 7) Змейка по кубу
void boingboing(uint16_t iterations, int delay, unsigned char mode, unsigned char drawmode)
{
	fill(0x00);	// Очистка куба

	int x, y, z; // Текущие координаты точки
	int dx, dy, dz;	// Направление движения
	int lol, i;	
	unsigned char crash_x, crash_y, crash_z;

	y = rand()%8;
	x = rand()%8;
	z = rand()%8;

	// Массив координат змейки.
	int snake[8][3];
	for (i=0;i<8;i++)
	{
		snake[i][0] = x;
		snake[i][1] = y;
		snake[i][2] = z;
	}
	
	
	dx = 1;
	dy = 1;
	dz = 1;
	
	while(iterations)
	{
		crash_x = 0;
		crash_y = 0;
		crash_z = 0;
		

		// Немного перемешаем
		if (rand()%3 == 0)
		{
			// Выберает случайную ось и установливает скорость на случайное число
			lol = rand()%3;
			if (lol == 0)
			dx = rand()%3 - 1;
			
			if (lol == 1)
			dy = rand()%3 - 1;
			
			if (lol == 2)
			dz = rand()%3 - 1;
		}

		// Точка достигла 0 по оси X и пытается перейти к -1,
		// если перейдёт к -1 будет сбой
		if (dx == -1 && x == 0)
		{
			crash_x = 0x01;
			if (rand()%3 == 1)
			{
				dx = 1;
			} else
			{
				dx = 0;
			}
		}
		
		// Точка достигла 0 по оси Y и пытается перейти к -1,
		// если перейдёт к -1 будет сбой
		if (dy == -1 && y == 0)
		{
			crash_y = 0x01;
			if (rand()%3 == 1)
			{
				dy = 1;
			} else
			{
				dy = 0;
			}
		}
		
		// Точка достигла 0 по оси Z и пытается перейти к -1
		// если перейдёт к -1 будет сбой
		if (dz == -1 && z == 0)
		{
			crash_z = 0x01;
			if (rand()%3 == 1)
			{
				dz = 1;
			} else
			{
				dz = 0;
			}
		}
		
		// Точка достигла 7 по оси X и пытается перейти к 8,
		// если перейдёт к 8 будет сбой
		if (dx == 1 && x == 7)
		{
			crash_x = 0x01;
			if (rand()%3 == 1)
			{
				dx = -1;
			} else
			{
				dx = 0;
			}
		}
		
		// Точка достигла 7 по оси Y и пытается перейти к 8,
		// если перейдёт к 8 будет сбой
		if (dy == 1 && y == 7)
		{
			crash_y = 0x01;
			if (rand()%3 == 1)
			{
				dy = -1;
			} else
			{
				dy = 0;
			}
		}
		
		// Точка достигла 7 по оси Z и пытается перейти к 8,
		// если перейдёт к 8 будет сбой
		if (dz == 1 && z == 7)
		{
			crash_z = 0x01;
			if (rand()%3 == 1)
			{
				dz = -1;
			} else
			{
				dz = 0;
			}
		}
		
		// mode bit 0 sets crash action enable
		if (mode | 0x01)
		{
			if (crash_x)
			{
				if (dy == 0)
				{
					if (y == 7)
					{
						dy = -1;
					} else if (y == 0)
					{
						dy = +1;
					} else
					{
						if (rand()%2 == 0)
						{
							dy = -1;
						} else
						{
							dy = 1;
						}
					}
				}
				if (dz == 0)
				{
					if (z == 7)
					{
						dz = -1;
					} else if (z == 0)
					{
						dz = 1;
					} else
					{
						if (rand()%2 == 0)
						{
							dz = -1;
						} else
						{
							dz = 1;
						}
					}
				}
			}
			
			if (crash_y)
			{
				if (dx == 0)
				{
					if (x == 7)
					{
						dx = -1;
					} else if (x == 0)
					{
						dx = 1;
					} else
					{
						if (rand()%2 == 0)
						{
							dx = -1;
						} else
						{
							dx = 1;
						}
					}
				}
				if (dz == 0)
				{
					if (z == 3)
					{
						dz = -1;
					} else if (z == 0)
					{
						dz = 1;
					} else
					{
						if (rand()%2 == 0)
						{
							dz = -1;
						} else
						{
							dz = 1;
						}
					}
				}
			}
			
			if (crash_z)
			{
				if (dy == 0)
				{
					if (y == 7)
					{
						dy = -1;
					} else if (y == 0)
					{
						dy = 1;
					} else
					{
						if (rand()%2 == 0)
						{
							dy = -1;
						} else
						{
							dy = 1;
						}
					}
				}
				if (dx == 0)
				{
					if (x == 7)
					{
						dx = -1;
					} else if (x == 0)
					{
						dx = 1;
					} else
					{
						if (rand()%2 == 0)
						{
							dx = -1;
						} else
						{
							dx = 1;
						}
					}
				}
			}
		}
		
		if (mode | 0x02)
		{
			if ( // Мы в одной из 8-ми угловых позиций
			(x == 0 && y == 0 && z == 0) ||
			(x == 0 && y == 0 && z == 7) ||
			(x == 0 && y == 7 && z == 0) ||
			(x == 0 && y == 7 && z == 7) ||
			(x == 7 && y == 0 && z == 0) ||
			(x == 7 && y == 0 && z == 7) ||
			(x == 7 && y == 7 && z == 0) ||
			(x == 7 && y == 7 && z == 7)
			)
			{
				// В этот момент воксель будет прыгать
				// вперед и назад между этим углом,
				// и прямо в противоположный угол.
				// Нам это не нужно!
				
				// Итак, мы немного меняем траекторию,
				// чтобы избежать залипания углов
				lol = rand()%3;
				if (lol == 0)
				dx = 0;
				
				if (lol == 1)
				dy = 0;
				
				if (lol == 2)
				dz = 0;
			}
		}

		// Последняя провека на правильность 
		if (x == 0 && dx == -1)
		dx = 1;
		
		if (y == 0 && dy == -1)
		dy = 1;
		
		if (z == 0 && dz == -1)
		dz = 1;
		
		if (x == 7 && dx == 1)
		dx = -1;
		
		if (y == 7 && dy == 1)
		dy = -1;
		
		if (z == 7 && dz == 1)
		dz = -1;
		
		
		// Наконец, перемещаем воксель
		x = x + dx;
		y = y + dy;
		z = z + dz;
		
		if (drawmode == 0x01) // Показываем один воксель за раз
		{
			setvoxel(x,y,z);
			delay_ms(delay);
			clrvoxel(x,y,z);
		} else if (drawmode == 0x02) // Переворачиваем воксель, с которым работаем
		{
			flpvoxel(x,y,z);
			delay_ms(delay);
		} if (drawmode == 0x03) // Рисуем змейку
		{
			for (i=7;i>=0;i--)
			{
				snake[i][0] = snake[i-1][0];
				snake[i][1] = snake[i-1][1];
				snake[i][2] = snake[i-1][2];
			}
			snake[0][0] = x;
			snake[0][1] = y;
			snake[0][2] = z;
			
			for (i=0;i<8;i++)
			{
				setvoxel(snake[i][0],snake[i][1],snake[i][2]);
			}
			delay_ms(delay);
			for (i=0;i<8;i++)
			{
				clrvoxel(snake[i][0],snake[i][1],snake[i][2]);
			}
		}
		
		
		iterations--;
	}
}

// 8) Зажигание всего куба по одному вокселю 
void effect_random_filler (int delay, int state)
{
	int x,y,z;
	int loop = 0;
	
	
	if (state == 1)
	{
		fill(0x00); // Очистка куба
	} else
	{
		fill(0xff); // Заполнение куба
	}
	
	while (loop<511) // Проверка заполнения всего куба
	{
		x = rand()%8;// Рандомное значение координаты X
		y = rand()%8;// Рандомное значение координаты Y
		z = rand()%8;// Рандомное значение координаты Z

		if ((state == 0 && getvoxel(x,y,z) == 0x01) || (state == 1 && getvoxel(x,y,z) == 0x00))
		{
			altervoxel(x,y,z,state);
			delay_ms(delay);
			loop++;
		}
	}
}

// Передавай число итераций (например: 100), но цикл закрыт в коде. 
// Я бы попробовал и так и с открытым циклом. Не знаю, что получится.
void effect_random(int count_iteration){ 
	int i, k, z;
	int rand_x, rand_y;
	
	//for(i=0; i<count_iteration; i++){
		
		for(z=0; z<8; z++){
			
			for(k=0; k<8; k++){
				rand_x = rand()%8;
				rand_y = rand()%8;
				setvoxel(rand_x, rand_y, z);
			}
			
		}
		
	//}
	
}

// 9) Эфект дождя 
void effect_rain (int iterations)
{
	int i, ii;
	int rnd_x;
	int rnd_y;
	int rnd_num;
	
	for (ii=0; ii<iterations; ii++)
	{
		rnd_num = rand()%4;
		
		for (i=0; i < rnd_num; i++)
		{
			rnd_x = rand()%8; // Рандомное значение для координаты X
			rnd_y = rand()%8; // Рандомное значение для координаты Y
			setvoxel(rnd_x,rnd_y,7); // Образует один слой с рандомными значениями координат
		}
		
		delay_ms(1000); // Задержка
		shift(AXIS_Z,-1); // Передвигает готовый слой по оси Z
	}
}

// 10) передвижение группы вокселей вверх/вниз (похож на дождь)
void effect_z_updown (int iterations, int delay)
{
	unsigned char positions[64];
	unsigned char destinations[64];

	int i,y,move;
	
	for (i=0; i<64; i++)
	{
		positions[i] = 4;
		destinations[i] = rand()%8;
	}

	for (i=0; i<8; i++)
	{
		effect_z_updown_move(positions, destinations, AXIS_Z);
		delay_ms(delay);
	}
	
	for (i=0;i<iterations;i++)
	{
		for (move=0;move<8;move++)
		{
			effect_z_updown_move(positions, destinations, AXIS_Z);
			delay_ms(delay);
		}

		delay_ms(delay*4);


		for (y=0;y<32;y++)
		{
			destinations[rand()%64] = rand()%8;
		}
		
	}

}

void effect_z_updown_move (unsigned char positions[64], unsigned char destinations[64], char axis)
{
	int px;
	for (px=0; px<64; px++)
	{
		if (positions[px]<destinations[px])
		{
			positions[px]++;
		}
		if (positions[px]>destinations[px])
		{
			positions[px]--;
		}
	}
	
	draw_positions_axis (AXIS_Z, positions,0);
}

// 11) Размазываются стенки куба по всему кубу
void effect_axis_updown_randsuspend (char axis, int delay, int sleep, int invert)
{
	unsigned char positions[64];
	unsigned char destinations[64];

	int i,px;
	
	// 64 набора случайных позиций
	for (i=0; i<64; i++)
	{
		positions[i] = 0; // Установка старовых позиций в 0
		destinations[i] = rand()%8; // Рандомное значение позиций 
	}

	// Проход по 8-ми итерациям (по всем слоям)
	for (i=0; i<8; i++)
	{
		// Для каждой итерации, перемещает все позиции на один шаг ближе к месту назначения
		for (px=0; px<64; px++)
		{
			if (positions[px]<destinations[px])
			{
				positions[px]++;
			}
		}
		// Обрисовка позиции
		draw_positions_axis (axis, positions,invert);
		delay_ms(delay); //задержка
	}
	
	// Назначение всем адресатам занчения 7 (противоположная сторона) 
	for (i=0; i<64; i++)
	{
		destinations[i] = 7;
	}
	
	// Задержка позиции
	delay_ms(sleep);
	
	// Затем делает тоже самое ещё раз
	for (i=0; i<8; i++)
	{
		for (px=0; px<64; px++)
		{
			if (positions[px]<destinations[px])
			{
				positions[px]++;
			}
			if (positions[px]>destinations[px])
			{
				positions[px]--;
			}
		}
		// Обрисовка позиции
		draw_positions_axis (axis, positions,invert); 
		delay_ms(delay);// Задержка
	}
}

void draw_positions_axis (char axis, unsigned char positions[64], int invert)
{
	int x, y, p;
	
	fill(0x00);
	
	for (x=0; x<8; x++)
	{
		for (y=0; y<8; y++)
		{
			if (invert)
			{
				p = (7-positions[(x*8)+y]);
			} else
			{
				p = positions[(x*8)+y];
			}
			
			if (axis == AXIS_Z)
			setvoxel(x,y,p);
			
			if (axis == AXIS_Y)
			setvoxel(x,p,y);
			
			if (axis == AXIS_X)
			setvoxel(p,y,x);
		}
	}
	
}

// 12) Перелет вокселей от одной стенки к другой (противоположной)
void effect_boxside_randsend_parallel (char axis, int origin, int delay, int mode)
{
	int i;
	int done;
	unsigned char cubepos[64];
	unsigned char pos[64];
	int notdone = 1;
	int notdone2 = 1;
	int sent = 0;
	
	for (i=0;i<64;i++)
	{
		pos[i] = 0;
	}
	
	while (notdone)
	{
		if (mode == 1)
		{
			notdone2 = 1;
			while (notdone2 && sent<64)
			{
				i = rand()%64;
				if (pos[i] == 0)
				{
					sent++;
					pos[i] += 1;
					notdone2 = 0;
				}
			}
		} else if (mode == 2)
		{
			if (sent<64)
			{
				pos[sent] += 1;
				sent++;
			}
		}
		
		done = 0;
		for (i=0;i<64;i++)
		{
			if (pos[i] > 0 && pos[i] <7)
			{
				pos[i] += 1;
			}
			
			if (pos[i] == 7)
			done++;
		}
		
		if (done == 64)
		notdone = 0;
		
		for (i=0;i<64;i++)
		{
			if (origin == 0)
			{
				cubepos[i] = pos[i];
			} else
			{
				cubepos[i] = (7-pos[i]);
			}
		}
		
		
		delay_ms(delay);
		draw_positions_axis(axis,cubepos,0);
		LED_PORT ^= LED_RED;
	}
	
}

// 13) Сверхнего слоя зажигается весь куб (по слоям), потом так же по слоям гаснет
void effect_loadbar(int delay)
{
	fill(0x00);// Очистка куба
	
	int z,y;
	
	// Цикл по слоям на зажигание светодиодов
	for (z=0;z<8;z++)
	{
		
		for (y=0;y<8;y++)
		cube[z][y] = 0xff;
		
		delay_ms(delay); // Задержка
	}
	
	delay_ms(delay*3);// Задержка
	
	// Цикл по слоям на потухание светодиодов
	for (z=0;z<8;z++)
	{
		for (y=0;y<8;y++)
		cube[z][y] = 0x00;
		
		delay_ms(delay);// Задержка
	}
}


// Установка n-го количество вокселей в случайных положениях
void effect_random_sparkle_flash (int iterations, int voxels, int delay)
{
	int i;
	int v;
	for (i = 0; i < iterations; i++)
	{
		for (v=0;v<=voxels;v++)
		setvoxel(rand()%8,rand()%8,rand()%8);
		
		delay_ms(delay);
		fill(0x00);
	}
}

// Мигает 1 случайный воксель, мигает 2 случайных вокселя ... мигает 20 
// случайных вокселей и снова возвращается к 1.
void effect_random_sparkle (void)
{
	int i;
	
	for (i=1;i<20;i++)
	{
		effect_random_sparkle_flash(5,i,200);
	}
	
	for (i=20;i>=1;i--)
	{
		effect_random_sparkle_flash(5,i,200);
	}
	
}

int effect_telcstairs_do(int x, int val, int delay)
{
	int y,z;

	for(y = 0, z = x; y <= z; y++, x--)
	{
		if(x < CUBE_SIZE && y < CUBE_SIZE)
		{
			cube[x][y] = val;
		}
	}
	delay_ms(delay);
	return z;
}

void effect_telcstairs (int invert, int delay, int val)
{
	int x;

	if(invert)
	{
		for(x = CUBE_SIZE*2; x >= 0; x--)
		{
			x = effect_telcstairs_do(x,val,delay);
		}
	}
	else
	{
		for(x = 0; x < CUBE_SIZE*2; x++)
		{
			x = effect_telcstairs_do(x,val,delay);
		}
	}
}

// 14) Эффект "смерч"
void effect_wormsqueeze (int size, int axis, int direction, int iterations, int delay)
{
	int x, y, i,j,k, dx, dy;
	int cube_size;
	int origin = 0;
	
	if (direction == -1)
	origin = 7;
	
	cube_size = 8-(size-1);
	
	x = rand()%cube_size;
	y = rand()%cube_size;
	
	for (i=0; i<iterations; i++)
	{
		dx = ((rand()%3)-1);
		dy = ((rand()%3)-1);
		
		if ((x+dx) > 0 && (x+dx) < cube_size)
		x += dx;
		
		if ((y+dy) > 0 && (y+dy) < cube_size)
		y += dy;
		
		shift(axis, direction);
		

		for (j=0; j<size;j++)
		{
			for (k=0; k<size;k++)
			{
				if (axis == AXIS_Z)
				setvoxel(x+j,y+k,origin);
				
				if (axis == AXIS_Y)
				setvoxel(x+j,origin,y+k);
				
				if (axis == AXIS_X)
				setvoxel(origin,y+j,x+k);
			}
		}
		
		delay_ms(delay);
	}
}

// Эффект смайлик
//void effect_smileyspin (int count, int delay, char bitmap)
//{
	//unsigned char dybde[] = {0,1,2,3,4,5,6,7,1,1,2,3,4,5,6,6,2,2,3,3,4,4,5,5,3,3,3,3,4,4,4,4};
	//int d = 0;
	//int flip = 0;
	//int x, y, off;
	//for(int i = 0; i<count; i++)
	//{
		//flip = 0;
		//d = 0;
		//off = 0;
		//// front:
		//for (int s=0;s<7;s++){
			//if(!flip){
				//off++;
				//if (off == 4){
					//flip = 1;
					//off = 0;
				//}
				//} else {
				//off++;
			//}
			//for (x=0; x<8; x++)
			//{
				//d = 0;
				//for (y=0; y<8; y++)
				//{
					//if (font_getbitmappixel ( bitmap, 7-x, y)){
						//if (!flip)
						//setvoxel(y,dybde[8 * off + d++],x);
						//else
						//setvoxel(y,dybde[31 - 8 * off - d++],x);
						//} else {
						//d++;
					//}
				//}
			//}
			//delay_ms(delay);
			//fill(0x00);
		//}
//
		//// side:
		//off = 0;
		//flip = 0;
		//d = 0;
		//for (int s=0;s<7;s++){
			//if(!flip){
				//off++;
				//if (off == 4){
					//flip = 1;
					//off = 0;
				//}
				//} else {
				//off++;
			//}
			//for (x=0; x<8; x++)
			//{
				//d = 0;
				//for (y=0; y<8; y++)
				//{
					//if (font_getbitmappixel ( bitmap, 7-x, y)){
						//if (!flip)
						//setvoxel(dybde[8 * off + d++], 7 - y,x);
						//else
						//setvoxel(dybde[31 - 8 * off - d++],7 - y,x);
						//} else {
						//d++;
					//}
				//}
			//}
			//delay_ms(delay);
			//fill(0x00);
		//}
//
//
		//flip = 0;
		//d = 0;
		//off = 0;
		//// back:
		//for (int s=0;s<7;s++){
			//if(!flip){
				//off++;
				//if (off == 4){
					//flip = 1;
					//off = 0;
				//}
				//} else {
				//off++;
			//}
			//for (x=0; x<8; x++)
			//{
				//d = 0;
				//for (y=0; y<8; y++)
				//{
					//if (font_getbitmappixel ( bitmap, 7-x, 7-y)){
						//if (!flip)
						//setvoxel(y,dybde[8 * off + d++],x);
						//else
						//setvoxel(y,dybde[31 - 8 * off - d++],x);
						//} else {
						//d++;
					//}
				//}
			//}
			//delay_ms(delay);
			//fill(0x00);
		//}
//
		//// other side:
		//off = 0;
		//flip = 0;
		//d = 0;
		//for (int s=0;s<7;s++){
			//if(!flip){
				//off++;
				//if (off == 4){
					//flip = 1;
					//off = 0;
				//}
				//} else {
				//off++;
			//}
			//for (x=0; x<8; x++)
			//{
				//d = 0;
				//for (y=0; y<8; y++)
				//{
					//if (font_getbitmappixel ( bitmap, 7-x, 7-y)){
						//if (!flip)
						//setvoxel(dybde[8 * off + d++], 7 - y,x);
						//else
						//setvoxel(dybde[31 - 8 * off - d++],7 - y,x);
						//} else {
						//d++;
					//}
				//}
			//}
			//delay_ms(delay);
			//fill(0x00);
		//}
//
	//}
//}


void effect_pathmove (unsigned char *path, int length)
{
	int i,z;
	unsigned char state;
	
	for (i=(length-1);i>=1;i--)
	{
		for (z=0;z<8;z++)
		{
			
			state = getvoxel(((path[(i-1)]>>4) & 0x0f), (path[(i-1)] & 0x0f), z);
			altervoxel(((path[i]>>4) & 0x0f), (path[i] & 0x0f), z, state);
		}
	}
	for (i=0;i<8;i++)
	clrvoxel(((path[0]>>4) & 0x0f), (path[0] & 0x0f),i);
}

//void effect_rand_patharound (int iterations, int delay)
//{
	//int z, dz, i;
	//z = 4;
	//unsigned char path[28];
	//
	//font_getpath(0,path,28);
	//
	//for (i = 0; i < iterations; i++)
	//{
		//dz = ((rand()%3)-1);
		//z += dz;
		//
		//if (z>7)
		//z = 7;
		//
		//if (z<0)
		//z = 0;
		//
		//effect_pathmove(path, 28);
		//setvoxel(0,7,z);
		//delay_ms(delay);
	//}
//}

void effect_pathspiral (int iterations, int delay)
{
	int i;
	//int z;
	//z = 4;
	unsigned char path[16];
	
	font_getpath(1,path,16);
	
	for (i = 0; i < iterations; i++)
	{
		setvoxel(4,0,i%8);
		delay_ms(delay);
		effect_pathmove(path, 28);
		
	}
}

void effect_path_text (int delay, char *str)
{
	int z, i,ii;
	z = 4;
	unsigned char path[28];
	font_getpath(0,path,28);
	
	unsigned char chr[5];
	unsigned char stripe;
	
	while (*str)
	{
		//charfly(*str++, direction, axis, mode, delay);
		
		
		font_getchar(*str++, chr);
		
		for (ii=0;ii<5;ii++)
		{
			//stripe = pgm_read_byte(&font[(chr*5)+ii]);
			stripe = chr[ii];
			
			for (z=0;z<8;z++)
			{
				if ((stripe>>(7-z)) & 0x01)
				{
					setvoxel(0,7,z);
				} else
				{
					clrvoxel(0,7,z);
				}
				
			}
			effect_pathmove(path, 28);
			delay_ms(delay);
		}
		
		effect_pathmove(path, 28);
		delay_ms(delay);
	}
	for (i=0;i<28;i++)
	{
		effect_pathmove(path, 28);
		delay_ms(delay);
	}
}


// Вращение буквы по граням куба
void effect_path_bitmap (int delay, char bitmap, int iterations)
{
	int z, i, ii;
	z = 4;
	unsigned char path[28];
	font_getpath(0,path,28);
	
	for (i=0; i < iterations; i++)
	{
		for (ii=0;ii<8;ii++)
		{
			for (z=0;z<8;z++)
			{
				if (font_getbitmappixel(bitmap,(7-z),ii))
				{
					setvoxel(0,7,z);
				} else
				{
					clrvoxel(0,7,z);
				}
				
			}
			delay_ms(delay);
			effect_pathmove(path, 28);
		}
		
		for (ii=0;ii<20;ii++)
		{
			delay_ms(delay);
			effect_pathmove(path, 28);
		}
	}
	for (ii=0;ii<10;ii++)
	{
		delay_ms(delay);
		effect_pathmove(path, 28);
	}
}



