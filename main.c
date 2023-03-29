#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <err.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "image_processing/greyscale.h"
#include "image_processing/binarisation_3.h"
#include "image_processing/contrast.h"
#include "image_processing/gras.h"
#include "image_processing/grid_detection.h"  
#include "image_processing/manual_rotation.h"
#include "image_processing/rotation_auto.h"
#include "image_processing/result_solver.h"
#include "solver/solver_sudoku.h"
#include "neural_network/xor.h"
#include "neural_network/matrix.h"
#include "image_processing/imgtolist.h"


typedef struct
{
	GtkBuilder *builder;
	gpointer user_data;
	gchar *filename;
} SGlobalData;
        
void callback_add(GtkMenuItem *menuitem, gpointer user_data);
void callback_validate(GtkMenuItem *menuitem, gpointer user_data);        
void callback_cancel(GtkMenuItem *menuitem, gpointer user_data);
void callback_save(GtkMenuItem *menuitem, gpointer user_data);
void callback_help(GtkMenuItem *menuitem, gpointer user_data);
void callback_gray(GtkMenuItem *menuitem, gpointer user_data);
void callback_binarize(GtkMenuItem *menuitem, gpointer user_data);
void callback_rotation(GtkMenuItem *menuitem, gpointer user_data);
void callback_detec(GtkMenuItem *menuitem, gpointer user_data);
void callback_solver(GtkMenuItem *menuitem, gpointer user_data);


int main(int argc, char *argv [])
{
        SGlobalData data;
        GError *error = NULL;
        gchar* load = NULL;
        data.filename=NULL;
 
        /* Init GDK library */
        gtk_init(&argc, &argv);
 
        /* Ouverture du fichier Glade de la fenêtre principale */
        data.builder = gtk_builder_new();
 
        /* Creation of the path to test.glade*/
        load =  g_build_filename ("test.glade", NULL);
 
        /* Loading of test.glade. */
        gtk_builder_add_from_file (data.builder, load, &error);
        g_free (load);
        if (gtk_builder_add_from_file(data.builder, "test.glade", &error) == 0)
    	{
		g_printerr("Error loading file: %s\n", error->message);
		g_clear_error(&error);
		return 1;
    	}
 
        /* Affectation des signaux de l'interface aux différents CallBacks. */
        gtk_builder_connect_signals (data.builder, &data);
 
        /* Récupération des pointeurs */
        GtkWidget *fenetre_principale = GTK_WIDGET(gtk_builder_get_object (data.builder, "MainWindow"));
        GtkWidget *ButtonImage = GTK_WIDGET(gtk_builder_get_object (data.builder, "ButtonImage"));
        GtkWidget *HelpButton = GTK_WIDGET(gtk_builder_get_object (data.builder, "HelpButton"));
        GtkWidget *ValidateButton = GTK_WIDGET(gtk_builder_get_object (data.builder, "ValidateButton"));
        GtkWidget *CancelButton = GTK_WIDGET(gtk_builder_get_object (data.builder, "CancelButton"));
        GdkPixbuf *image_pix = NULL;
        GtkImage *image = GTK_IMAGE(gtk_builder_get_object(data.builder,"Photo"));
        GtkWidget *GrayButton = GTK_WIDGET(gtk_builder_get_object (data.builder, "GrayButton"));
        GtkWidget *ButtonBinarize = GTK_WIDGET(gtk_builder_get_object (data.builder, "ButtonBinarize"));
        GtkWidget *ButtonRotate = GTK_WIDGET(gtk_builder_get_object (data.builder, "ButtonRotate"));
        GtkWidget *ButtonDetec = GTK_WIDGET(gtk_builder_get_object (data.builder, "ButtonDetec"));
        GtkWidget *ButtonSolver = GTK_WIDGET(gtk_builder_get_object (data.builder, "ButtonSolver"));
        
        //Connect signal
        g_signal_connect (G_OBJECT(ButtonImage), "clicked", G_CALLBACK(callback_add), &data);
        g_signal_connect (G_OBJECT(ValidateButton), "clicked", G_CALLBACK(callback_validate), &data);
        g_signal_connect (G_OBJECT(CancelButton), "clicked", G_CALLBACK(callback_cancel), &data);
        g_signal_connect(G_OBJECT(HelpButton), "clicked", G_CALLBACK(callback_help), &data);
        g_signal_connect(G_OBJECT(GrayButton), "clicked", G_CALLBACK(callback_gray), &data);
        g_signal_connect(G_OBJECT(ButtonBinarize), "clicked", G_CALLBACK(callback_binarize), &data);
        g_signal_connect(G_OBJECT(ButtonRotate), "clicked", G_CALLBACK(callback_rotation), &data);
        g_signal_connect(G_OBJECT(ButtonDetec), "clicked", G_CALLBACK(callback_detec), &data);
        g_signal_connect(G_OBJECT(ButtonSolver), "clicked", G_CALLBACK(callback_solver), &data);
        
        /* Affichage de la fenêtre principale. */
        gtk_widget_show_all (fenetre_principale);
        g_signal_connect (G_OBJECT (fenetre_principale), "destroy", (GCallback)gtk_main_quit, NULL);
        
        
        gtk_main();
 
        return 0;
}

//leave the application
void callback_quit(GtkMenuItem *menuitem, gpointer user_data)
{
  menuitem = menuitem;
  user_data=user_data;
  gtk_main_quit();
}

//Open the about window
void callback_help(GtkMenuItem *menuitem, gpointer user_data)
{
        gint res;
        SGlobalData *data = (SGlobalData*) user_data;
        GtkWidget *about = NULL;
        about = GTK_WIDGET (gtk_builder_get_object (data->builder, "about"));        
        res = gtk_dialog_run (GTK_DIALOG (about));
        res = res;
        
        gtk_widget_hide(about);

}

//Add button to choose a photo
void callback_add(GtkMenuItem *menuitem, gpointer user_data)
{

	gint res;
        SGlobalData *data = (SGlobalData*) user_data;
        GtkWidget *add = NULL;
        add = GTK_WIDGET (gtk_builder_get_object (data->builder, "FileChooserWindow"));
        
        res = gtk_dialog_run (GTK_DIALOG (add));
        res=res;
}

 
//Validate the photo chosen
void callback_validate(GtkMenuItem *menuitem, gpointer user_data)
{
        SGlobalData *data = (SGlobalData*) user_data;
        GtkWidget *chooserdialog = NULL;
        chooserdialog=GTK_WIDGET (gtk_builder_get_object (data->builder, "FileChooserWindow"));
 
 	data->filename=NULL;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(chooserdialog);
        data->filename = gtk_file_chooser_get_filename(chooser);
 
        //PIXBUFF
        GError **error=NULL;
        GdkPixbuf *image_pix = gdk_pixbuf_new_from_file_at_size(data->filename, 400, 400, error);
 
        //affichage de l'image
        GtkImage *image = GTK_IMAGE(gtk_builder_get_object(data->builder,"Photo"));
        gtk_image_set_from_pixbuf (image, image_pix);
 
	remove("grayscale.png");
	remove("binarized.png");
	remove("rotated.png");
	remove("contrasted.png");
	remove("detected.png");
	remove("filled.png");
        gtk_widget_hide(chooserdialog);
}
 
 //Cancel the selection
void callback_cancel(GtkMenuItem *menuitem, gpointer user_data)
{
        SGlobalData *data = (SGlobalData*) user_data;
        GtkWidget *chooserdialog = NULL;
 
        chooserdialog=GTK_WIDGET (gtk_builder_get_object (data->builder, "FileChooserWindow"));
        gtk_widget_hide(chooserdialog);
}

void callback_gray(GtkMenuItem *menuitem, gpointer user_data)
{
	SGlobalData *data = (SGlobalData*) user_data;
	if (data->filename!=NULL)
	{
		//Object Result in glade
		GtkImage *Result = GTK_IMAGE(gtk_builder_get_object(data->builder,"Result"));
		SDL_Surface* image = IMG_Load(data->filename);
		
		//greyscale
		greyscale(image);
		IMG_SavePNG(image,"grayscale.png");
		
		//PIXBUFF
		GError **error=NULL;
		GdkPixbuf *image_pix = gdk_pixbuf_new_from_file_at_size("grayscale.png", 400, 400, error);
		
		gtk_image_set_from_pixbuf (Result, image_pix);
	}
	
}

void callback_binarize(GtkMenuItem *menuitem, gpointer user_data)
{
	SGlobalData *data = (SGlobalData*) user_data;
	if (data->filename!=NULL)
	{
		//Object Result in glade
		GtkImage *Result = GTK_IMAGE(gtk_builder_get_object(data->builder,"Result"));
		FILE * fichier = fopen("binarized.png", "r+"); 
		
		//Check if the binarized version already exists
		if (fichier == NULL)
		{
			SDL_Surface* image = IMG_Load(data->filename);
			
			//greyscale
			greyscale(image);
			IMG_SavePNG(image,"grayscale.png");
			
			//Constrast
			contrast(image,3);
			IMG_SavePNG(image,"contrasted.png");

			//Binarize
			binarisation_3(image);
			IMG_SavePNG(image,"binarized.png");
			
			//PIXBUFF
			GError **error=NULL;
			GdkPixbuf *image_pix = gdk_pixbuf_new_from_file_at_size("binarized.png", 400, 400, error);
			
			gtk_image_set_from_pixbuf (Result, image_pix);
		}
		else
		{		        
			//PIXBUFF
			GError **error=NULL;
			GdkPixbuf *image_pix = gdk_pixbuf_new_from_file_at_size("binarized.png", 400, 400, error);
			gtk_image_set_from_pixbuf (Result, image_pix);
		        fclose(fichier);
		}		
	}
}



void callback_rotation(GtkMenuItem *menuitem, gpointer user_data)
{
	SGlobalData *data = (SGlobalData*) user_data;
	if(data->filename!=NULL)
	{
		//Object Result in glade
		GtkImage *Result = GTK_IMAGE(gtk_builder_get_object(data->builder,"Result"));
		
		FILE * fichier = fopen("binarized.png", "r+"); 
		if (fichier == NULL)
		{
			SDL_Surface* image = IMG_Load(data->filename);
			
			//greyscale
			greyscale(image);
			IMG_SavePNG(image,"grayscale.png");
			
			//Constrast
			contrast(image,3);
			IMG_SavePNG(image,"contrasted.png");

			//Binarize
			binarisation_3(image);
			IMG_SavePNG(image,"binarized.png");
			
			gras(image);
			int angle = rotateauto(image);
			image = manual_rotation_with_redim(image,angle);
			
			IMG_SavePNG(image,"rotated.png");
		
			//PIXBUFF
			GError **error=NULL;
			GdkPixbuf *image_pix = gdk_pixbuf_new_from_file_at_size("rotated.png", 400, 400, error);
			
			gtk_image_set_from_pixbuf (Result, image_pix);
		}
		else
		{
		        SDL_Surface* image = IMG_Load("binarized.png");
			g_print("Optmisation \n");
		        gras(image);
		        g_print("1 - Gras fait \n");
			int angle = rotateauto(image);
			 g_print("2 - Angle trouvé \n");
			image = manual_rotation_with_redim(image,angle);
			
			IMG_SavePNG(image,"rotated.png");
		
			//PIXBUFF
			GError **error=NULL;
			GdkPixbuf *image_pix = gdk_pixbuf_new_from_file_at_size("rotated.png", 400, 400, error);
			
			gtk_image_set_from_pixbuf (Result, image_pix);
		        fclose(fichier);
		}		
	}
}


//Enregistre le résultat de la résolution de l'ocr
void callback_save(GtkMenuItem *menuitem, gpointer user_data)
{

        SGlobalData *data = (SGlobalData*) user_data;
      /*  GtkWidget *chooserdialog = NULL;
        chooserdialog=GTK_WIDGET (gtk_builder_get_object (data->builder, "FileSaveWindow")); 

        char *filename;
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(chooserdialog)); */

        //IMPLEMENTER ICI LA FONCTION POUR RECUPERER L'IMAGE
        /*gint count = 0;
        GtkTextIter start;
        GtkTextIter end;*/
       /*    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_builder_get_object(data->builder, "TextView")));
        buffer.set_text(texte);
        gtk_text_buffer_get_start_iter (buffer, &start);
        gtk_text_buffer_get_end_iter (buffer, &end);
        count = gtk_text_buffer_get_char_count (buffer);
        //saveas(filename, buffer, start, end, count);

        gtk_widget_hide (chooserdialog); */
}


void callback_detec(GtkMenuItem *menuitem, gpointer user_data)
{
	SGlobalData *data = (SGlobalData*) user_data;
	if(data->filename!=NULL)
	{
		//Object Result in glade
		GtkImage *Result = GTK_IMAGE(gtk_builder_get_object(data->builder,"Result"));
		
		FILE * fichier = fopen("binarized.png", "r+"); 
		if (fichier == NULL)
		{
			SDL_Surface* image = IMG_Load(data->filename);				
			//greyscale
			greyscale(image);
			IMG_SavePNG(image,"grayscale.png");
			
			//Constrast
			contrast(image,3);
			IMG_SavePNG(image,"contrasted.png");

			//Binarize
			binarisation_3(image);
			IMG_SavePNG(image,"binarized.png");
			
			gras(image);
			int angle = rotateauto(image);
			image = manual_rotation_with_redim(image,angle);
			IMG_SavePNG(image,"rotated.png");
			
			grid_detection(image);
			IMG_SavePNG(image,"detec.png");
			
			//PIXBUFF
			GError **error=NULL;
			GdkPixbuf *image_pix = gdk_pixbuf_new_from_file_at_size("detec.png", 400, 400, error);
			
			gtk_image_set_from_pixbuf (Result, image_pix);
		}
		else
		{
			FILE * file2 = fopen("rotated.png", "r+"); 
			if (file2 == NULL)
			{
				SDL_Surface* image = IMG_Load("binarized.png");
				g_print("Optmisation \n");
				gras(image);
				g_print("1 - Gras fait \n");
				int angle = rotateauto(image);
				 g_print("2 - Angle trouvé \n");
				image = manual_rotation_with_redim(image,angle);
				IMG_SavePNG(image,"rotated.png");
				
				grid_detection(image);
				IMG_SavePNG(image,"detec.png");
				
				//PIXBUFF
				GError **error=NULL;
				GdkPixbuf *image_pix = gdk_pixbuf_new_from_file_at_size("detec.png", 400, 400, error);
				
				gtk_image_set_from_pixbuf (Result, image_pix);
			}
			else
			{
				SDL_Surface* image = IMG_Load("rotated.png");
				grid_detection(image);
				IMG_SavePNG(image,"detec.png");
				
				//PIXBUFF
				GError **error=NULL;
				GdkPixbuf *image_pix = gdk_pixbuf_new_from_file_at_size("detec.png", 400, 400, error);
				
				gtk_image_set_from_pixbuf (Result, image_pix);
				fclose(file2);
			}
		        fclose(fichier);
		}		
	}
}

void callback_solver(GtkMenuItem *menuitem, gpointer user_data)
{
	SGlobalData *data = (SGlobalData*) user_data;
	if(data->filename!=NULL)
	{
		//Object Result in glade
		GtkImage *Result = GTK_IMAGE(gtk_builder_get_object(data->builder,"Result"));
		SDL_Surface* test = IMG_Load("bank_numbers/empty.jpg");
		
		result_solver(test);
		IMG_SavePNG(test,"filled.jpg");
		
		//PIXBUFF
		GError **error=NULL;
		
		GdkPixbuf *image_pix = gdk_pixbuf_new_from_file_at_size("filled.jpg", 400, 400, error);
		gtk_image_set_from_pixbuf (Result, image_pix);
		
		
	}
}

