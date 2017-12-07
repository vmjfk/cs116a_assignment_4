
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define MAIN_WINDOW_WIDTH 640
#define MAIN_WINDOW_HEIGHT 480

int window_height,window_width;

struct drawSpec {
    int shapeType; // 0=rect,1=line,2=ellipse,3=bezier
    int isFilled; // 1 = filled, 0 = not filled
    int complete; //1=complete
    int pointsEntered; //number of points that have been entered. shows how close we are to complete. 
    GLfloat r,g,b;
    int x[4];
    int y[4];
};
struct drawSpec shapeQueue[100];
int totalShapes = 0;

void printDrawSpec(struct drawSpec spec)
{
    printf("printDrawSpec called.\n");
    printf("shapeType = %d\n",spec.shapeType);
    printf("isFilled = %d\n",spec.isFilled);
    printf("complete = %d\n",spec.complete);
    printf("points Entered = %d\n",spec.pointsEntered);
    printf("r,g,b = %f, %f, %f\n",spec.r,spec.g,spec.b);
    for(int i=0;i<spec.pointsEntered;i++)
    {
        printf("point %d: x = %d, y = %d\n",i,spec.x[i],spec.y[i]);
    }
    
}
GLenum errorCheck()
{
    GLenum code;
    const GLubyte *string;
    code = glGetError();
    if(code == GL_NO_ERROR)
    {
        string = gluErrorString(code);
        fprintf(stderr,"GL Error = %s\n",string);
    }
    return code;
}
void drawRect(struct drawSpec shape)
{
    printf("drawRect called\n");
    printDrawSpec(shape);
    GLenum gl_mode;
    if(shape.isFilled == 0)
    {
        gl_mode = GL_LINE_LOOP;
    }
    else
    {
        gl_mode = GL_POLYGON;
    }
    glBegin (gl_mode);
    glColor3f (shape.r,shape.g,shape.b);
  
    glVertex2i (shape.x[0],shape.y[0]);
    glVertex2i (shape.x[1],shape.y[0]);
    glVertex2i (shape.x[1],shape.y[1]);
    glVertex2i (shape.x[0],shape.y[1]);

    glEnd ();
    errorCheck();

}
void drawLine(struct drawSpec shape)
{
    printf("drawLine called\n");
    printDrawSpec(shape);
    GLenum gl_mode;
    gl_mode = GL_LINES;
    glBegin (gl_mode);
    glColor3f (shape.r,shape.g,shape.b);
  
    glVertex2i (shape.x[0],shape.y[0]);
    glVertex2i (shape.x[1],shape.y[1]);

    glEnd ();
    errorCheck();

}
void drawEllipse(struct drawSpec shape)
{
    printf("drawEllipse called\n");
    double theta = 0.0; 
    printDrawSpec(shape);
    GLenum gl_mode;
    if(shape.isFilled == 0)
    {
        gl_mode = GL_LINE_LOOP;
    }
    else
    {
        gl_mode = GL_POLYGON;
    }
    glBegin (gl_mode);
    glColor3f (shape.r,shape.g,shape.b);
    int center_x = shape.x[0];
    int center_y = shape.y[0];
    int radius_x = shape.x[1] - shape.x[0];
    int radius_y = shape.y[1] - shape.y[0];

    for(theta = 0;theta < 2 * M_PI;theta+=2*M_PI / 360)
    {
        printf("theta = %f\n",theta);
        int x = center_x + radius_x*cos(theta);
        int y = center_y + radius_y*sin(theta);
    
        glVertex2i (x,y);
    }
    glEnd ();
    errorCheck();
    
}

void drawBezier(struct drawSpec shape)
{
    printf("drawBezier called\n");
    printDrawSpec(shape);
    int C[4];
    C[0] = 1;
    C[1] = 3;
    C[2] = 3;
    C[3] = 1;
    
    GLenum gl_mode;
    gl_mode = GL_LINE_STRIP;
    glBegin (gl_mode);
    glColor3f (shape.r,shape.g,shape.b);
    for(int i=0;i<4;i++)
    {
        printf("bezier point %d x = %d , y = %d\n",i,shape.x[i],shape.y[i]);

    }
    for(float t=0;t<1.0;t+=0.05) //20 bez points
    {
        GLfloat x, y;
        x = 0.0;
        y = 0.0;
        for(int i = 0 ; i < 4;i++) //4 th degree bez
        {
            x += pow((1-t),3-i)*shape.x[i]*C[i]*pow(t,i);
            y += pow((1-t),3-i)*shape.y[i]*C[i]*pow(t,i);
        }
        glVertex2i (x,y);
        //printf("bezier x = %f,y = %f\n",x,y);

    }
    glEnd ();
    errorCheck();

}
void loadShapes()
{
    // loops through the shape queue and call a draw program for each one. 
    // determines which draw to do by the "shapetype" attribute of the drawspec. 
    for(int i=0;i<totalShapes;i++)
    {
        if(shapeQueue[i].complete == 1)
        {
            switch (shapeQueue[i].shapeType) {
                case 0:
                    drawRect(shapeQueue[i]);
                    break;
                case 1: 
                    drawLine(shapeQueue[i]);
                    break;
                case 2:
                    drawEllipse(shapeQueue[i]);
                    break;
                case 3:
                    drawBezier(shapeQueue[i]);
                    break;
                default:
                    break;
            }
        }
    }

}


void mouse(int button, int state, int x, int y)
{
    printf("button = %d, state = %d, x = %d, y = %d\n",button,state,x,y);

    if(button != 2)return;//only works with right mouse button
    if(state != 0)return; //only on down mouse not up  mouse

    int currentShape = totalShapes;
    if(shapeQueue[totalShapes].complete == 0) // don't do anything without a spec waiting to be filled
    {

        shapeQueue[totalShapes].x[shapeQueue[totalShapes].pointsEntered] = x;
        shapeQueue[totalShapes].y[shapeQueue[totalShapes].pointsEntered] = y;
        shapeQueue[totalShapes].pointsEntered++;
        int pointsNeeded = 0;
        switch(shapeQueue[totalShapes].shapeType) // 0=rect,1=line,2=ellipse,3=bezier
        {
            case 0: //needs two points
            case 1: //needs two points
            case 2: //needs two points
                pointsNeeded = 2;
                break;
            case 3: //needs four points
                pointsNeeded = 4;
                break;
            default:
                break;
        }
        if(shapeQueue[totalShapes].pointsEntered == pointsNeeded)
        {
            shapeQueue[totalShapes].complete = 1;
            totalShapes++;
        }
    }
    printDrawSpec(shapeQueue[currentShape]);
    glutPostRedisplay();

 
}
void mainMenu(int value)
{  
    // this gets called after a menu choice
    // creates a new drawSpec 
    // sets the "complete" flag to 0;
    
    printf("mainMenu called: %d\n",value);
    
    switch(value / 10)
    {
        case 0: //filled rectangle
            shapeQueue[totalShapes].shapeType = 0;
            shapeQueue[totalShapes].isFilled = 1;
            break;
        case 1: //unfilled rectangle
            shapeQueue[totalShapes].shapeType = 0;
            shapeQueue[totalShapes].isFilled = 0;
            break;
        case 2: //filled ellipse
            shapeQueue[totalShapes].shapeType = 2;
            shapeQueue[totalShapes].isFilled = 1;
            break;
        case 3: //unfilled ellipse
            shapeQueue[totalShapes].shapeType = 2;
            shapeQueue[totalShapes].isFilled = 0;
            break;
        case 4: //line
            shapeQueue[totalShapes].shapeType = 1;
            shapeQueue[totalShapes].isFilled = 0;
            break;
        case 5: // bezier
            shapeQueue[totalShapes].shapeType = 3;
            shapeQueue[totalShapes].isFilled = 0;
            break;
        default:
            break;
    }
    switch(value % 10)
    {
        case 0: //red
            shapeQueue[totalShapes].r = 1.0;
            shapeQueue[totalShapes].g = 0.0;
            shapeQueue[totalShapes].b = 0.0;
            break;
        case 1: //green
            shapeQueue[totalShapes].r = 0.0;
            shapeQueue[totalShapes].g = 1.0;
            shapeQueue[totalShapes].b = 0.0;
            break;
        case 2: //blue 
            shapeQueue[totalShapes].r = 0.0;
            shapeQueue[totalShapes].g = 0.0;
            shapeQueue[totalShapes].b = 1.0;
            break;
        case 3: //yellow
            shapeQueue[totalShapes].r = 1.0;
            shapeQueue[totalShapes].g = 1.0;
            shapeQueue[totalShapes].b = 0.0;
            break;
        case 4: //purple
            shapeQueue[totalShapes].r = 1.0;
            shapeQueue[totalShapes].g = 0.0;
            shapeQueue[totalShapes].b = 1.0;
            break;
        case 5: //orange
            shapeQueue[totalShapes].r = 1.0;
            shapeQueue[totalShapes].g = 0.5;
            shapeQueue[totalShapes].b = 0.0;
            break;
        case 6: //white
            shapeQueue[totalShapes].r = 1.0;
            shapeQueue[totalShapes].g = 1.0;
            shapeQueue[totalShapes].b = 1.0;
            break;
        case 7: //cyan
            shapeQueue[totalShapes].r = 0.0;
            shapeQueue[totalShapes].g = 1.0;
            shapeQueue[totalShapes].b = 1.0;
            break;
    }

    shapeQueue[totalShapes].complete = 0;
    shapeQueue[totalShapes].pointsEntered = 0;
    printDrawSpec(shapeQueue[totalShapes]);



}

void init(void)
{


    glClearColor(0.0,0.0,0.0,0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,MAIN_WINDOW_WIDTH,MAIN_WINDOW_HEIGHT,0);


}

void display()
{
    printf("display called\n");

    glClear (GL_COLOR_BUFFER_BIT);

    loadShapes();
    glFlush();
}


void reshape(int width, int height)
{
    printf("reshape called. width = %d, height = %d\n",width,height);
    window_height = height;
    window_width = width;

 //   gluOrtho2D(0,width,0,height);
//    glutPostRedisplay();

//	glViewport( 0, 0, width, height );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,width,height,0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}
void buildMenus()
{
    printf("buildMenus called.\n");

 
    char* color[8];
    color[0] = "red";
    color[1] = "green";
    color[2] = "blue";
    color[3] = "yellow";
    color[4] = "purple";
    color[5] = "orange";
    color[6] = "white";
    color[7] = "cyan";

    
    int ctr = 0;
    int rectFilledColorMenu = glutCreateMenu(mainMenu);
    for(int i = 0;i<8;i++)
    {
        glutAddMenuEntry(color[i],i + ctr*10);
    }

    ctr++;
    int rectOutlineColorMenu = glutCreateMenu(mainMenu);
    for(int i = 0;i<8;i++)
    {
        glutAddMenuEntry(color[i],i + ctr*10);
    }

    ctr++;
    int ellipseFilledColorMenu = glutCreateMenu(mainMenu);
    for(int i = 0;i<8;i++)
    {
        glutAddMenuEntry(color[i],i + ctr*10);
    }
    
    ctr++;
    int ellipseOutlineColorMenu = glutCreateMenu(mainMenu);
    for(int i = 0;i<8;i++)
    {
        glutAddMenuEntry(color[i],i + ctr*10);
    }

    ctr++;
    int lineMenu = glutCreateMenu(mainMenu);
    for(int i = 0;i<8;i++)
    {
        glutAddMenuEntry(color[i],i + ctr*10);
    }

    ctr++;
    int bezMenu = glutCreateMenu(mainMenu);
    for(int i = 0;i<8;i++)
    {
        glutAddMenuEntry(color[i],i + ctr*10);
    }

    int rectMenu = glutCreateMenu(mainMenu);
    glutAddSubMenu("filled",rectFilledColorMenu);
    glutAddSubMenu("outline",rectOutlineColorMenu);

    int ellipseMenu = glutCreateMenu(mainMenu);
    glutAddSubMenu("filled",ellipseFilledColorMenu);
    glutAddSubMenu("outline",ellipseOutlineColorMenu);

 

    int mnMenu = glutCreateMenu(mainMenu);
    glutAddSubMenu("rectangle",rectMenu);
    glutAddSubMenu("ellipse",ellipseMenu);
    glutAddSubMenu("line",lineMenu);
    glutAddSubMenu("bezier",bezMenu);


    
    glutAttachMenu(GLUT_LEFT_BUTTON);
}

int main (int argc, char *argv[])
{  
    window_height = MAIN_WINDOW_HEIGHT;
    window_width = MAIN_WINDOW_WIDTH;

	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
	glutInitWindowPosition (0,0);
	glutCreateWindow ("MyPaint");
    
    buildMenus();

    init();	
    glutDisplayFunc (display);
	glutReshapeFunc (reshape);
	glutMouseFunc (mouse);


  
	glutMainLoop ();
}

