#include <GL/gl.h>
#include <GL/glu.h>
#include <gtk/gtk.h>
#include <gtk/gtkgl.h>

static gboolean
canvas_expose(GtkWidget *widget, GdkEvent *event, gpointer data){
	GdkGLDrawable *drawable;
	GdkGLContext *context;
	gint width, height;

	gdk_drawable_get_size(GDK_DRAWABLE(widget->window),&width,&height);
	drawable = gtk_widget_get_gl_drawable(widget);
	context = gtk_widget_get_gl_context(widget);

	if(gdk_gl_drawable_gl_begin(drawable,context)){
		glViewport(0,0,width,height);
		glPushMatrix();

		if(width > height){
			glScalef((double) height/width, 1.0, 1.0);
		}else{
			glScalef((double) width/height, 1.0, 1.0);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor3f(1.0,1.0,1.0);
		glBegin(GL_LINE_STRIP);
		glVertex2f(-0.5,-0.5);
		glVertex2f(0.5,-0.5);
		glVertex2f(-0.5,0.5);
		glVertex2f(0.5,0.5);
		glEnd();

		glPopMatrix();
		glFlush();
		gdk_gl_drawable_wait_gl(drawable);
		gdk_gl_drawable_swap_buffers(drawable);
		gdk_gl_drawable_gl_end(drawable);
	}
	return TRUE;
}

int main(int argc, char *argv[])
{
	GtkWidget *window;
	GtkWidget *canvas;

	int attr_list[] = {
		GDK_GL_RGBA,
		GDK_GL_DOUBLEBUFFER,
		GDK_GL_DEPTH_SIZE,1,
		GDK_GL_ATTRIB_LIST_NONE
	};

	GdkGLConfig *glconfig;

	gtk_init(&argc, &argv);
	gdk_gl_init(&argc, &argv);
	gtk_gl_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "glwindow");
	gtk_window_set_default_size(GTK_WINDOW(window), 400,400);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	canvas = gtk_drawing_area_new();
	glconfig = gdk_gl_config_new(attr_list);
	gtk_widget_set_gl_capability(canvas, glconfig, NULL, TRUE, GDK_GL_RGBA);

	g_signal_connect(canvas, "expose-event", G_CALLBACK(canvas_expose),NULL);
	gtk_container_add(GTK_CONTAINER(window), canvas);

	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}
