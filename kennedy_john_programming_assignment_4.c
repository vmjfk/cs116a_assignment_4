
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>


#define MAIN_WINDOW_WIDTH 640
#define MAIN_WINDOW_HEIGHT 480


GLdouble eyeX, eyeY, eyeZ, refX, refY, refZ, upX, upY, upZ;

int gl_mode;
float r,g,b;
char* colorNames[8];


struct Vertex2d {
    GLfloat x;
    GLfloat y; 
};

struct Color {
    GLfloat r,g,b;
};


struct Shape {
    struct Vertex2d points[100];
    struct Color color;
    int gl_mode; //  glBegin (GL_POLYGON); or GL_LINE
  
    int shapeSize;
};

struct Shape shapeStack[100];
int shapeStackHeight = 0;
int shapeType;
int gl_mode;


void loadShapes()
{
    printf("loadShapes called \n");
 
    glClear(GL_COLOR_BUFFER_BIT);
    for(int i = 0;i<shapeStackHeight;i++)
    {
        struct Shape shape = shapeStack[i];
        glColor3f(shape.color.r,shape.color.g,shape.color.b);
           
        glBegin(shape.gl_mode); //GL_POLYGON for filled,  GL_LINE for unfilled or bez or line
        
        for(int j=0;j<shape.shapeSize;j++)
        {
            glVertex2f(shape.points[j].x,shape.points[j].y);
        }
        glEnd();
    }

}

void reshow()
{

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    printf("button = %d, state = %d, x = %d, y = %d\n",button,state,x,y);

}
void mainMenu(int value)
{

    int colorIndex = value %10;
    int shapeType = value /10;
    switch(shapeType)
    {
        case 0: //filed rect
            gl_mode = GL_POLYGON;
            break;
        case 2: //filled ellipse
            gl_mode = GL_POLYGON;
            break;
        default: // otherwise it's a line
            gl_mode = GL_LINE;
    }
    
    switch (colorIndex)
    {
        case 0:
            r = 1.0f;
            g = 0.0f;
            b = 0.0f;
            break;
        case 1:
            r = 0.0f;
            g = 1.0f;
            b = 0.0f;
            break;
        case 2:
            r = 0.0f;
            g = 0.0f;
            b = 1.0f;
            break;
        case 3:
            r = 0.0f;
            g = 1.0f;
            b = 1.0f;
            break;
        case 4:
            printf("case 4\n");
            r = 1.0f;
            g = 0.0f;
            b = 1.0f;
            break;
        case 5:
            r = 1.0f;
            g = 1.0f;
            b = 0.0f;
            break;
        case 6:
            r = 0.0f;
            g = 0.0f;
            b = 0.0f;
            break;
        case 7:
            r = 1.0f;
            g = 1.0f;
            b = 1.0f;
            break;
        default:
            printf("default\n");
            break;
    }
    
 
    

    printf("r = %f, g = %f, b = %f\n",r,g,b);
    printf("shapeType = %d\n",shapeType);
    printf("filled = %d\n",gl_mode);
    
    
}

void init(void)
{
    colorNames[0] = "red";
    colorNames[1] = "green";
    colorNames[2] = "blue";
    colorNames[3] = "yellow";
    colorNames[4] = "purple";
    colorNames[5] = "orange";
    colorNames[6] = "white";
    colorNames[7] = "black";

    glClearColor(0.0,0.0,0.0,0.0);
 

    gl_mode = GL_LINE;
    r = 0.0f;
    g = 1.0f;
    b = 0.0f;

}

void display()
{
    printf("display called\n");
    glClear(GL_COLOR_BUFFER_BIT);

    loadShapes();
    glFlush();
    glutSwapBuffers();

}


void reshape(int width, int height)
{
    printf("reshape called.\n");
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat aspect = (GLfloat) width/(GLfloat)height;
    gluPerspective(45.0f,aspect, 0.1f,100.0f);  
    glMatrixMode(GL_MODELVIEW);
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
    color[7] = "black";

    
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

	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
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

