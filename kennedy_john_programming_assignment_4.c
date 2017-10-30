
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>



#define MAIN_WINDOW_WIDTH 640
#define MAIN_WINDOW_HEIGHT 480


GLdouble eyeX, eyeY, eyeZ, refX, refY, refZ, upX, upY, upZ;

int gl_mode;
float r,g,b;
 

void loadWorld()
{
    printf("loadWorld called \n");
 
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(r,g,b);
    glPolygonMode(GL_FRONT_AND_BACK,gl_mode);
    glBegin(GL_TRIANGLES);
    printf("loading %d faces\n",face_count);
        for(int i=0;i<face_count;i++)
        {
      //      printf("adding face %d:\n",i);
            face f = faces[i];
    //        printf("face loaded\n");
            for(int j=0;j<3;j++)
            {
            
                int index = f.verts[j] - 1;
  //              printf("index :%d\n",index);
                vertex3d v = vertices[index];
                GLfloat x,y,z;
                x = v.x;
                y = v.y;
                z = v.z;
 //               glNormal3f(0.0f,0.0f,1.0f);
                glVertex3f(x,y,z);
            }
        }
    glEnd();


}

void reshow()
{
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, refX, refY, refZ, upX, upY, upZ);
    glutPostRedisplay();
}


void init(void)
{

    glClearColor(0.0,0.0,0.0,0.0);
 

    glMatrixMode(GL_MODELVIEW);
    eyeX = 0.0f;
    eyeY = 0.1f;
    eyeZ = 0.5f;
    refX = 0.0f; 
    refY = 0.1f; 
    refZ = 0.0f; 
    upX = 0.0f; 
    upY = 1.0f; 
    upZ = 0.0f;
    gl_mode = GL_LINE;
    r = 0.0f;
    g = 1.0f;
    b = 0.0f;
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, refX, refY, refZ, upX, upY, upZ);

}

void display()
{
    printf("display called\n");
    glClear(GL_COLOR_BUFFER_BIT);

    loadWorld();
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

int main (int argc, char *argv[])
{  

	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
	glutInitWindowPosition (0,0);
	glutCreateWindow ("MyPaint");

    init();	
    glutDisplayFunc (display);
	glutReshapeFunc (reshape);
	
	glutKeyboardFunc (keyboard);
	glutSpecialFunc (arrow_keys);

  
	glutMainLoop ();
}

