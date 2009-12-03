//John McAlarney 2009
//LCD Simulator

#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
//#include <iverilog/vpi_user.h>
#include <vpi_user.h>

//prototypes
void initWindow();
void displayFn();
void idleFn();
void reshapeFn();
void drawPxl(int, int, int, int, int);
void init();
void initLCD();
void init2();
void plot_register();
static int plot_compiletf(char*user_data);
static int plot_calltf(char*user_data);

void init_lcd_register();
static int init_lcd_compiletf(char*user_data);
static int init_lcd_calltf(char*user_data);

void redraw_register();
static int redraw_compiletf(char*user_data);
static int redraw_calltf(char*user_data);

//texture object
GLuint screenTexture[1];

//lcd monitor
void lcd_monitor(int, int, int, int, int);

//main window
int mainWindow;

//texture data
unsigned char * texData;

//current x and Y for the raster
int pX, pY;

unsigned int screenW;
unsigned int screenH;

unsigned char texel[3];

unsigned int scanCnt = 0;

int refreshSpeed;

void drawPxl(int x, int y, int r, int g, int b)
{
	if(x<0) x=0;
	if(x>screenW-1) x=screenW-1;
	if(y<0) y=0;
	if(y>screenH-1) y=screenH-1;
	y=screenH-y-1;
	int tOffset = (y*screenW+x)*3;
	//usleep(33);
	//displayFn();
	
	
	texData[tOffset]=(unsigned char)r;
	texData[tOffset+1]=(unsigned char)g;
	texData[tOffset+2]=(unsigned char)b;
	
	//texel[0]=texData[tOffset];
	//texel[1]=texData[tOffset + 1];
	//texel[2]=texData[tOffset + 2];
	//glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, texel);
	//texData[tOffset+3]=0xff;

	//texData[391677] = 0xff;
	//texData[391678] = 0xff;
	//texData[391679] = 0xff;
	
	scanCnt++;
	if(scanCnt >= screenW * screenH * 2 || !refreshSpeed)
	{
		scanCnt=0;
		displayFn();
	}
	
}

void reshapeFn()
{
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	glViewport(0, 0, w, h);
	gluOrtho2D(-1,1,-1,1);
}

void keyboardFn(unsigned char key, int x, int y)
{
	if(key=='q' || key=='Q') exit(0);
}



void idleFn()
{
}


void displayFn()
{
	int x,y, tOffset;
	
	//glBindTexture(GL_TEXTURE_2D, screenTexture[0]);
	// when texture area is small, bilinear filter the closest mipmap
  	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 	// when texture area is large, bilinear filter the first mipmap
 	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexImage2D(GL_TEXTURE_2D, 0 , 3 , screenW , screenH , 0 , GL_RGB , GL_UNSIGNED_BYTE , texData );
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, screenW, screenH, GL_RGB, GL_UNSIGNED_BYTE, texData);
	//glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, screenW, screenH, GL_RGB, GL_UNSIGNED_BYTE, texData);
	
	//glEnable(GL_TEXTURE_2D);
	
	//reshape the window in case it has been resized
	reshapeFn();
	
	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	
	// select our current texture
 	//glBindTexture(GL_TEXTURE_2D, screenTexture);
	
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0); glVertex2d(-1, -1);
	glTexCoord2d(1.0, 0.0); glVertex2d(1, -1);
	glTexCoord2d(1.0, 1.0); glVertex2d(1, 1);
	glTexCoord2d(0.0, 1.0); glVertex2d(-1, 1);
	glEnd();
	
	
	glFlush();
  glutSwapBuffers();
}

void init()
{
	int i=0;
	
	//set the clear color to black
  glClearColor(0.0, 0.0, 0.0, 1.0);
 
 	glDisable (GL_LINE_SMOOTH);	
 	 
  //initialize screen texture
  glGenTextures(1, &screenTexture[0]);
  
  //allocate memory for the texture data
	texData = (unsigned char*)malloc(screenW*screenH*4);
  
  // select our current textureint argc = 1;
  glBindTexture(GL_TEXTURE_2D, screenTexture[0]);
  
  // select modulate to mix texture with color for shading
  //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  
  //linear as an option?
  // when texture area is small, bilinear filter the closest mipmap
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // when texture area is large, bilinear filter the first mipmap
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
  // texture should tile
  //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 	
 	glEnable(GL_TEXTURE_2D);
 	
 	//zero out texture data
 	for(i=0;i<screenW*screenH*3;i++)
	{
		texData[i] = 0;
	}
 	
 	// build our texture mipmaps
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, screenW, screenH, GL_RGB, GL_UNSIGNED_BYTE, texData);
  //glTexImage2D(GL_TEXTURE_2D, 0 , 3 , screenW , screenH , 0 , GL_RGB , GL_UNSIGNED_BYTE , texData );
 	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, screenW, screenH, GL_RGB, GL_UNSIGNED_BYTE, texData);


}

void init2()
{
	//set the clear color to black
  glClearColor(0.0, 0.0, 0.0, 1.0);



}

void initLCD()
{
	int argc_sim = 1;
	char *argv_sim[] = {"lcdsim", NULL};
	
	//initialize GLUT
	glutInit(&argc_sim, argv_sim);
	
	//set window size to 640x480 initially
	glutInitWindowSize(screenW,screenH);
	
	//initialize a single bufferedered display window
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	
	//create a window
	mainWindow = glutCreateWindow("LCD Sim");
	
	init();
	
	//displayFn();	
		
	//set the display function
	//glutDisplayFunc(displayFn);
	//set the idle Function
	//glutIdleFunc(idleFn);
	//keyboard function
	//glutKeyboardFunc(keyboardFn);
	//glutReshapeFunc(reshapeFn);
	
	//drawPxl(240,136,255,255,255);
	
	

	
	//drawPxl(479,271,255,255,255);
	//drawPxl(240,136,255,255,255);
	
	//glutMainLoop();
}

static int plot_compiletf(char*user_data)
{
  return 0;
}

static int plot_calltf(char*user_data)
{
	vpiHandle systfref, args_iter, argh;
  struct t_vpi_value argval;
  int x,y,r,g,b;
 
  // Obtain a handle to the argument list
  systfref = vpi_handle(vpiSysTfCall, NULL);
  args_iter = vpi_iterate(vpiArgument, systfref);
 
  // Grab x argument
  argh = vpi_scan(args_iter);
  argval.format = vpiIntVal;
  vpi_get_value(argh, &argval);
  x = argval.value.integer;
  
  // Grab y argument
  argh = vpi_scan(args_iter);
  argval.format = vpiIntVal;
  vpi_get_value(argh, &argval);
  y = argval.value.integer;
  
  // Grab r argument
  argh = vpi_scan(args_iter);
  argval.format = vpiIntVal;
  vpi_get_value(argh, &argval);
  r = argval.value.integer;
  
  // Grab y argument
  argh = vpi_scan(args_iter);
  argval.format = vpiIntVal;
  vpi_get_value(argh, &argval);
  g = argval.value.integer;
  
  // Grab y argument
  argh = vpi_scan(args_iter);
  argval.format = vpiIntVal;
  vpi_get_value(argh, &argval);
  b = argval.value.integer;
  
  // Cleanup and return
  vpi_free_object(args_iter);


	drawPxl(x,y,r,g,b);
	return 0;
}

void plot_register()
{
	s_vpi_systf_data tf_data;

  tf_data.type      = vpiSysTask;
  tf_data.tfname    = "$plot";
  tf_data.calltf    = plot_calltf;
  tf_data.compiletf = plot_compiletf;
  tf_data.sizetf    = 0;
  tf_data.user_data = 0;
  vpi_register_systf(&tf_data);
}


void (*vlog_startup_routines[])() = {
	init_lcd_register,
	redraw_register,
	plot_register,
	0
};

void init_lcd_register()
{
	s_vpi_systf_data tf_data;

  tf_data.type      = vpiSysTask;
  tf_data.tfname    = "$init_lcd";
  tf_data.calltf    = init_lcd_calltf;
  tf_data.compiletf = init_lcd_compiletf;
  tf_data.sizetf    = 0;
  tf_data.user_data = 0;
  vpi_register_systf(&tf_data);

}
static int init_lcd_compiletf(char*user_data)
{
	return 0;
}
static int init_lcd_calltf(char*user_data)
{
	vpiHandle systfref, args_iter, argh;
  struct t_vpi_value argval;
 
  // Obtain a handle to the argument list
  systfref = vpi_handle(vpiSysTfCall, NULL);
  args_iter = vpi_iterate(vpiArgument, systfref);
 
  // Grab x argument
  argh = vpi_scan(args_iter);
  argval.format = vpiIntVal;
  vpi_get_value(argh, &argval);
  screenW = argval.value.integer;
  
  // Grab y argument
  argh = vpi_scan(args_iter);
  argval.format = vpiIntVal;
  vpi_get_value(argh, &argval);
  screenH = argval.value.integer;
  
  // Grab refresh speed argument
  argh = vpi_scan(args_iter);
  argval.format = vpiIntVal;
  vpi_get_value(argh, &argval);
  refreshSpeed = argval.value.integer;
  
  // Cleanup and return
  vpi_free_object(args_iter);
  
  initLCD();
  //glutMainLoop();
  return 0;

}

void redraw_register()
{
	s_vpi_systf_data tf_data;
  tf_data.type      = vpiSysTask;
  tf_data.tfname    = "$redraw";
  tf_data.calltf    = redraw_calltf;
  tf_data.compiletf = redraw_compiletf;
  tf_data.sizetf    = 0;
  tf_data.user_data = 0;
  vpi_register_systf(&tf_data);

}
static int redraw_compiletf(char*user_data)
{
	return 0;
}
static int redraw_calltf(char*user_data)
{
	//glutPostRedisplay();
		//initialize screen texture
	displayFn();
	return 0;

}

