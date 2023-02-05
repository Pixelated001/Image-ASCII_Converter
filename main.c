#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


// Structures

typedef struct RGB
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
}RGB;

typedef struct rgbc
{
    int r;
    int g;
    int b;
    int c;
}rgbc;

typedef struct dimensions
{
    int w;
    int h;
}dimensions;

typedef struct corrdinates
{
    int x;
    int y;
}corrdinates;


// All the typedef operations 

typedef SDL_Surface* Surface;
typedef SDL_Window* Window;
typedef FILE* file;
typedef TTF_Font* FONT;


// Prototypes of functions

void create_window( dimensions , Window* , Surface* );
dimensions load_image( Surface* , char* );

char *convert( Surface* , Surface , Surface* , int );
rgbc Block_Editing(corrdinates, int, int, Uint32 *, Surface*);

void Text_Output (char *,char *, FONT, Surface *);
void Coloured_Image(Surface* , dimensions , FONT , RGB , char *, RGB);
void Coloured_Text_Image(Surface*, FONT, char *, Surface *);

void Save_Surface(Surface, char *);
void file_number_increase (char *, char *, int, char **);

/* Source based from Eike Anderson starts here; 
Originates from Programing Principles Lecture 19 pg 38 
and states the number of pixels a ASCII character takes up */
char letters[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','1','2','3','4','5','6','7','8','9','0'};  
int nums[] = {21,25,18,25,24,19,28,24,14,15,25,16,30,21,20,27,27,18,21,17,19,17,25,20,21,21,25,29,21,26,29,25,27,31,18,19,28,20,36,24,20,25,28,30,28,24,27,22,30,26,23,24,2,16,19,20,23,23,23,16,26,23,24};
/* Source based of Eike Anderson ends here */ 

/*
The enter and exit function of the program
parameters
    argc = number of arguments passed
    argv = contains all the command line arguments 
*/
int main ( int argc , char *argv[])
{
    /* Source from https://youtu.be/RSdXG5Gc0uc; 
    Declares surfaces that hold image and sets a surface to act as a canvas in window */
    Surface imageSurface = NULL; 
    Surface windowSurface = NULL;
    /* Source based on https://youtu.be/RSdXG5Gc0uc ends here */
    
    Surface imageConvertedSurface = NULL;
    Surface TextSurface = NULL;
    Surface BackGround_Surface = NULL;

    Window window = NULL;
    
    SDL_Event windowEvent;

    char *string = NULL;

    // counts how many times main has been called for sequences 
    static int frame_number = 1;

    // a simple booleon to check weather the function is processing 1 image or a sequence of them
    int repeat = 0;

    // Initialise both SDL and TTF so they can be used
    
    if (SDL_Init(SDL_INIT_EVERYTHING)<0)
    {
        printf("SDL couldnt be loaded error %s", SDL_GetError() );
    }

    if (TTF_Init()<0)
    {
        printf("TIFF couldnt be initzilised %s", SDL_GetError() );
    } 
    
    dimensions dim = load_image(&imageSurface, argv[1]);

    Surface Lower_Res = SDL_CreateRGBSurface(0, dim.h, dim.w, 32, 0, 0, 0, 0);       

    create_window(dim, &window, &windowSurface);

    SDL_Rect textLocation = {dim.h,0,0,0}; // Source based of https://gist.github.com/stavrossk/5004111; creates allocate a location to render the surface to the window
 
    
    if (! strcmp(argv[2], "-1"))
    {
        int Res = atoi(argv[4]);  
        Surface Text_Surface = NULL;
        FONT font = TTF_OpenFont( "lucon.ttf", Res);

        string = convert( &imageConvertedSurface , imageSurface , &Lower_Res , Res );
        Text_Output(argv[3], string, font, &Text_Surface);
        
        SDL_BlitSurface(Text_Surface, NULL, windowSurface, &textLocation); // Source based of https://gist.github.com/stavrossk/5004111; renders the text onto the window at the location of the rect 
        
        TTF_CloseFont (font);
        font = NULL;
        // if the user has chosen to pick the sequence option then this changes the name of the file output and file input 
        if (argc > 5)
        {
            if (! strcmp(argv[5], "-s"))
            {
                char *filename;
                frame_number ++;
                // increases the inputed file number and reassigns it to the same location in argv 
                file_number_increase(argv[1], ".bmp", frame_number, &filename);
                strcpy(argv[1], filename);
                //increases the output file number and reassigns it to the same location in argv
                file_number_increase(argv[3], ".txt", frame_number, &filename);
                strcpy(argv[3], filename);
                repeat = 1;
            }
        }
    }
    else if (! strcmp(argv[2], "-2"))
    {
        int Res = atoi(argv[10]);
        FONT font = TTF_OpenFont( "lucon.ttf", Res);
        RGB Text_Colour = {atoi(argv[4]), atoi(argv[5]), atoi(argv[6])};
        RGB Background_Colour = {atoi(argv[7]), atoi(argv[8]), atoi(argv[9])};

        string = convert( &imageConvertedSurface , imageSurface , &Lower_Res , Res );
        Coloured_Image(&BackGround_Surface, dim, font, Text_Colour, string, Background_Colour);
        
        SDL_BlitSurface(BackGround_Surface, NULL, windowSurface, &textLocation); // Source based of https://gist.github.com/stavrossk/5004111; renders the text onto the window at the location of the rect        
        Save_Surface(BackGround_Surface, argv[3]);
        
        TTF_CloseFont (font);
        font = NULL;
        // if the user has chosen to pick the sequence option then this changes the name of the file output and file input 
        if (argc > 11)
        {
            if (! strcmp(argv[11], "-s"))
            {
                char *filename;
                frame_number ++;
                // increases the inputed file number and reassigns it to the same location in argv 
                file_number_increase(argv[1], ".bmp", frame_number, &filename);
                strcpy(argv[1], filename);
                //increases the output file number and reassigns it to the same location in argv
                file_number_increase(argv[3], ".png", frame_number, &filename);
                strcpy(argv[3], filename);
                repeat = 1;
            }
        }
    }
    else if (! strcmp(argv[2], "-3"))
    {
        int Res = atoi(argv[4]);
        FONT font = TTF_OpenFont( "lucon.ttf", Res);
        
        string = convert( &imageConvertedSurface , imageSurface , &Lower_Res , Res );
        Coloured_Text_Image(&Lower_Res, font, string, &TextSurface);
        
        SDL_BlitSurface(TextSurface, NULL, windowSurface, &textLocation); // Source based of https://gist.github.com/stavrossk/5004111; renders the text onto the window at the location of the rect       
        Save_Surface(TextSurface, argv[3]);
        
        TTF_CloseFont (font);
        font = NULL;
        // if the user has chosen to pick the sequence option then this changes the name of the file output and file input 
        if (argc > 5)
        {
            if (! strcmp(argv[5], "-s"))
            {
                char *filename;
                frame_number ++;
                // increases the inputed file number and reassigns it to the same location in argv 
                file_number_increase(argv[1], ".bmp", frame_number, &filename);
                strcpy(argv[1], filename);
                //increases the output file number and reassigns it to the same location in argv
                file_number_increase(argv[3], ".png", frame_number, &filename);
                strcpy(argv[3], filename);
                repeat = 1;
            }
        }
    }
    else
    {
        printf("This output option is not avaliable");
        return 0;
    }

    
    /* 
    source based on https://youtu.be/M4Jgz0wEQxY; 
    loop continuesly to keep the window open and checks if any events occur for example closing the window
    */
    while (1)
    {
        if (SDL_PollEvent( &windowEvent ))
        {
            if (SDL_QUIT == windowEvent.type) 
            {
                break;
            }
        }
        SDL_BlitSurface(imageSurface, NULL, windowSurface, NULL);
        SDL_UpdateWindowSurface (window); 
    }
    /* source based on https://youtu.be/M4Jgz0wEQxY ends here */

    // Frees surfaces once they arnt being used any more 

    /* 
    Source from https://youtu.be/RSdXG5Gc0uc;
    releases the data storing these surface 
    */
    SDL_FreeSurface (imageSurface); 
    SDL_FreeSurface (windowSurface); 
    /* source based on https://youtu.be/RSdXG5Gc0uc ends here */
    
    SDL_FreeSurface (imageConvertedSurface); 
    SDL_FreeSurface (TextSurface);
    SDL_FreeSurface (BackGround_Surface);
    SDL_FreeSurface (Lower_Res);
    
    // These varaibles are pointers thefore its unsafe to not have it not assigned to something
    
    imageSurface = NULL; 
    windowSurface = NULL;
    imageConvertedSurface = NULL;
    TextSurface = NULL;
    BackGround_Surface = NULL;
    Lower_Res = NULL;

    /* 
    Source from https://youtu.be/M4Jgz0wEQxY; 
    closes window and closes SDL 
    */
    SDL_DestroyWindow(window); 
    SDL_Quit( );
    /* source based on https://youtu.be/M4Jgz0wEQxY ends here */
    
    /* if the progrma is converting a sequence of images then main 
    is called again with the updated comand line arguments done by 
    the function file_name_increase */
    if(repeat == 1)
    {
        main(argc, argv);
    }

}

/* 
Groups all the code neded to create window allowing it to be called multiple times if needed
This is called from main
Paramters:
    dim = The dimensions of the chosen image to convert so the window can be created with same height and double width of the chosen image
    window = A pointer to the window to assign the created window
    windowSurface = The surface assigned to the window 
*/
void create_window(dimensions dim, Window* window, Surface* windowSurface)
{
    /* 
    source based on https://youtu.be/M4Jgz0wEQxY; 
    Creates the window 
    */
    if ((*window =SDL_CreateWindow ("ASCII Converter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, dim.w, dim.h, SDL_WINDOW_ALLOW_HIGHDPI)) == NULL)
    {
        printf("Cannot create window %s", SDL_GetError());
    }
    /* source based on https://youtu.be/M4Jgz0wEQxY ends here */

    /* 
    Source from https://youtu.be/RSdXG5Gc0uc; 
    Asigns the window surface into the window to act as a canvas
    */
    *windowSurface = SDL_GetWindowSurface(*window);
    /* source based on https://youtu.be/M4Jgz0wEQxY ends here */
}

/*
Loads the users chosen image so that i can load multiple images if needed by calling this function again
Called in main
Parameters
    imageSurface = The surface passed by refrence to assign the chosen image to so it can be displayed in the window 
    filein = The name of the image that needs to be loaded 
*/
dimensions load_image(Surface* imageSurface, char* filein)
{
    dimensions dim;

    
    if (strstr(filein, ".bmp"))
    {
        /* 
        Source based on https://youtu.be/RSdXG5Gc0uc;
        Loads an image and saves it onto a surface 
        */
        if ((*imageSurface = SDL_LoadBMP(filein)) == NULL)
        {
            printf("Coudlnt Load image %s", SDL_GetError());
        }
        /* source based on https://youtu.be/M4Jgz0wEQxY ends here */
    }
    else if (strstr(filein, ".tif"))
    {
        if ((*imageSurface = IMG_Load(filein)) == NULL)
        {
            printf("Coudlnt Load image %s", SDL_GetError());
        }
    }
    //if (strstr(filein, ".png"))

    dim.w = ((*imageSurface)->w)*2; 
    dim.h = ((*imageSurface)->h);

    return dim;
}

/*
Contains the code to convert the image to ASCII. Groups the code to keep it clean and readable 
Called in main
Paramters
    imageConvertedSurface = The converted black and white version of the original image
    imageSurface = Contains the original image to get data from
    Lower_Res = The surface passed by refrence that will contain the lower resolution version of the image to use as text colour
    Res = The height of each grid block that the original image is split up to in
*/

char *convert(Surface* imageConvertedSurface, Surface imageSurface, Surface* Lower_Res, int Res)
{
    Uint32 pixel, *pixels = NULL;
    corrdinates cor;
    double a;

    // string to store all the ASCII characters in
    char string[90301] = "0";

    /*
    Source based on Eike Anderson starts here; 
    originates from Lab 8 task 5 converts the image surface into a pixel format that i can get RGBA data from
    */    
    *imageConvertedSurface = SDL_ConvertSurfaceFormat(imageSurface, SDL_PIXELFORMAT_ARGB8888, 0);
    pixels = (Uint32*) (*imageConvertedSurface)->pixels;
    /* Source based on Eike Anderson ends here */

    int edge = 0;
    
    edge = Edge_Res(Res);
    
    // creates a rect that is the same size of the blocks so that i create a low resolution version of the initial image by filling the rect at diffren poitions
    SDL_Rect Col_Location;
    Col_Location.x = 0;
    Col_Location.y = 0;
    Col_Location.w = edge;
    Col_Location.h = Res;

    int x = 0;
    int y = 0;
    int c = 0;
    int count = Res * edge;

    Uint32 col;

    // iterates through the top left poistion of each block of pixels returning the string of ASCII characters
    for (cor.y=0, Col_Location.y = 0;cor.y<(*imageConvertedSurface)->w;cor.y+=Res, Col_Location.y+=Res)
    {
        for (cor.x=0, Col_Location.x = 0;cor.x<(*imageConvertedSurface)->h;cor.x+=edge, Col_Location.x+=edge)
        {
            RGB colour;
            rgbc Colour_And_BW;

            Colour_And_BW = Block_Editing(cor, Res, edge, pixels, imageConvertedSurface);

            colour.r = Colour_And_BW.r / count;
            colour.g = Colour_And_BW.g / count;
            colour.b = Colour_And_BW.b / count;
            c = Colour_And_BW.c/count;
            
            col = SDL_MapRGB((*imageConvertedSurface)->format, colour.r,colour.g,colour.b);

            SDL_FillRect(*Lower_Res, &Col_Location, col);
            
            int letter_index = search(c); 

            strncat(string, &(letters[letter_index]), 1);
        }
        char t = '\n';
        strncat(string, &t, 1);     
    }
    return string;
}

/* 
Contains all the if statemnets for the width of the grid block dependant on which Res (height) is chosen by the user
Used in convert
Parameter
    Res = The resolution picekd by the user
*/

int Edge_Res(int Res)
{
    int edge = 0;
    if (Res == 24 || Res == 23)
    {
        return edge = 15;
    }
    else if (Res == 22 || Res == 21)
    {
        return edge = 14;
    }
    else if (Res == 20 || Res == 19)
    {
        return edge = 12;
    }
    else if (Res == 18)
    {
        return edge = 11;
    }
    else if (Res == 17 || Res == 16)
    {
        return edge = 10;
    }
    else if (Res == 15)
    {
        return edge = 9;
    }
    else if (Res == 14 || Res == 13)
    {
        return edge = 8;
    }
    else if (Res == 12 || Res == 11)
    {
        return edge = 7;
    }
    else if (Res == 10)
    {
        return edge = 6;
    }
    else if (Res == 9 || Res == 8)
    {
        return edge = 5;
    }
    else if (Res == 7 || Res == 6)
    {
        return edge = 4;
    }
    else if (Res == 5)
    {
        return edge = 3;
    }
    else if (Res == 4 || Res == 3)
    {
        return edge = 2;
    }
    else 
    {
        printf("This resolution is not reconised. Please enter a resolution between 3-24."); 
        return 0;
    }
    return 0;
}

/*
This contains the code to cycle through the pixels with in each block in the grid that splits up the image. 
This fucntion is used to tidy up the code and make it more readerable 

Called from convert

Paramters
    cor = The corrdinates of the top left pixel of the block wich is incimented in convert
    Res = Height of the block so i know how many rows to cycle through
    edge = The width of the grid so i know how many pixels across to cycle through in each row
    pixels = An array containing all the pxiel data from the orginal image so i can get the rgb from each pixel
    imageConvertedSurface = Contains the rgb pixel format of the orignal image
*/

rgbc Block_Editing(corrdinates cor, int Res, int edge, Uint32 *pixels, Surface* imageConvertedSurface)
{
    RGB colour; //rgb values 
    rgbc Colour_And_BW = {0,0,0,0};

    // iterates through the pixels with in the block at a given location and returns the average RGB and brightness value
    for (int y=cor.y; y<cor.y+Res; y++)
    {
        for (int x=cor.x; x<cor.x+edge; x++)
        {
            /* 
            Source based on Eike Anderson starts here; 
            originates from Lab 8 task 5 gets the RGBA values from a pixel in an array containing all the pixels from the image to be converted
            */
            SDL_GetRGB(pixels[x+y*(*imageConvertedSurface)->w],(*imageConvertedSurface)->format,&colour.r,&colour.g,&colour.b); //SDL get RGB (the pixel position assigned to x,y, the format of the data, the rgb variables to assign the got rgb)
            /* Source based on Eike Anderson ends here */

            Colour_And_BW.r += colour.r;
            Colour_And_BW.g += colour.g;
            Colour_And_BW.b += colour.b; 
                    
            int a = (0.212671*colour.r) + (0.715160*colour.g) + (0.072169*colour.b); //source based on Eike Anderson. Originates from progrmaing princples lecture 16 page 18

            Colour_And_BW.c += a;
        }
    }
    return Colour_And_BW;
}

/*
If the first output option is chosen this is called and writes the ASCII image to a .txt file
Called from main
Parameters
    file_name = The name of the .txt file the user wants to save the ASCII to
    string = The string containing the ASCII charecters
    font = The font to print the ASCII charecters
    Text_Surface = A surface to write all the ASCII charecter to so i can display the saved result in the SDL window   
*/
void Text_Output (char *file_name,char *string, FONT font, Surface *Text_Surface)
{
    // writes the ASCII characters to a surface 
    SDL_Color Text_Colour = {255, 255, 255};
    *Text_Surface = TTF_RenderText_Blended_Wrapped(font, string, Text_Colour, 500); // Source based of https://gist.github.com/stavrossk/5004111. Renders text onto a surface
    
    // saves the text surface into a given .txt file
    file text = NULL;
    if (! (text = fopen(file_name,"w")))
    {
        printf("couldnt open file hi");
    }
    fprintf(text, "%s", string);
    fclose(text);
}

/*
If the second output option is chosen then this function is called. With in the function a background surface is created and filled with the chosen bakcground colour.
Then writes ASCII charecter stored in string onto a surface. Finally the two surfaces are overlayed to get a adjustable background colour and font colour.
Called from main
Parameters
    BackGround_Surface = The surface to contain the background colour
    dim = Contains the width and heigh dimensions of the chosen image
    font = The font used to render the text to the surface
    Text_ColourRGB = A struct containing the inputed colour from the user
    string = The string of ASCII charecters to be rendered to surface
    Background_Colour = A structure containing the user inputed colour for the background
*/

void Coloured_Image(Surface* BackGround_Surface, dimensions dim, FONT font, RGB Text_ColourRGB, char *string, RGB Background_Colour)
{
    // creates a surface and fills it with the background colour
    *BackGround_Surface = SDL_CreateRGBSurface(0, dim.h, dim.w/2, 32, 0, 0, 0, 0);
    Uint32 white = SDL_MapRGB((*BackGround_Surface)->format, Background_Colour.r, Background_Colour.g, Background_Colour.b);
    SDL_FillRect(*BackGround_Surface, NULL, white);

    // writes the ASCII characters to a surface
    SDL_Color Text_Colour = {Text_ColourRGB.r, Text_ColourRGB.g, Text_ColourRGB.b};
    Surface Text_Surface = TTF_RenderText_Blended_Wrapped(font, string, Text_Colour, 500); // Source based of https://gist.github.com/stavrossk/5004111. Renders text onto a surface
    SDL_SetSurfaceBlendMode(Text_Surface, SDL_BLENDMODE_BLEND);

    // overlays the ASCII characters ontop of the background surface
    SDL_BlitSurface(Text_Surface, NULL, *BackGround_Surface, NULL);
    SDL_FreeSurface (Text_Surface);
    Text_Surface = NULL;
}

/*
If the 3rd output option is picekd this function is called and renders the string containing the ASCII to a surface then uses the alpha 
and applies it to the Low res of the original image to cut out the ASCII chareters from that surface creating colour ASCII charecters.
Called by main
Parameters
    Lower_Res = A surface containing a low res version of the orginal image full of block colours
    font = The font to render the ASCII to the text surface
    string = Contains the ASCII characters ysed to make up the image 
    TextSurface = The surface to render the ASCII to 
*/
void Coloured_Text_Image(Surface *Lower_Res, FONT font, char *string, Surface *TextSurface)
{
    // uses the alpha from the text surface to cut out the ASCII charecters from the lower resooution version of the original image 
    SDL_Color Text_Colour = {0, 0, 0};
    *TextSurface = TTF_RenderText_Blended_Wrapped(font, string, Text_Colour, 500); // Source based of https://gist.github.com/stavrossk/5004111. Renders text onto a surface
    SDL_SetSurfaceBlendMode(*TextSurface, SDL_BLENDMODE_ADD);
    SDL_SetSurfaceBlendMode(*Lower_Res, SDL_BLENDMODE_ADD);
    SDL_BlitSurface(*Lower_Res, NULL, *TextSurface, NULL);
}

/*
This function saves the inputed surface into a chosen file type.
Called by main
Parameters
    Save_Surface = The surface to be saved
    file_name = The name of the file to save to 
*/
void Save_Surface(Surface Save_Surface, char * file_name)
{
    // saves the inputted surface to given image file
    if (strstr(file_name, ".png"))
    {
        IMG_SavePNG(Save_Surface, file_name);
    }
    else
    {
        printf("This file type is not reconised!");
    }    
}

/*
This function calculates which ASCII characters is closest to the brightness value inputed
parameters
    Brightness = the average brightness value of a block of pixels
    returns the index of the array element that the Character is at in the array letters 
*/
int search(int Brightness)
{
    int i;
    int array_size = sizeof(nums)/sizeof(nums[0]);
    int increase = (Brightness/7.2)+2;
    int decrease =(Brightness/7.2)+2;
    // iterates through the array of brightness values and if one is not found then it increases and decreases the brightness value and checks again. repeated until value found 
    do
    {
        for(i=0; i<array_size; i++)
        {
            if (increase == nums[i] || decrease == nums[i])
            {
                return i;
            }
        }
        increase ++;
        decrease --;
    }while(1);
}

/* 
This functions is used if a sequence of images are chosen. the function increases 
the number on the image by one so that next time the program loads an image with alterd number. Eg mickey1 -> function -> mickey2 etc
Parameters
    string = the string containing the locaiton of the file inputted
    file_type = the type of file .bmp or .txt etc
    frame_number = the number of wich image throuhg the sequence the program is on
    filename = the variable to return the editied file location
*/
void file_number_increase (char *file_location, char *file_type, int frame_number, char **filename)
{
    char dest[100];
    // removes the number and type from the end of the string
    strncpy(dest, file_location, strlen(file_location)-5);
    dest[strlen(file_location)-5] = '\0';
    // adds the new number and file type to string
    sprintf(dest, "%s%d%s", dest, frame_number, file_type);
    dest[strlen(dest)] = '\0';
    // returns new string via refrence
    *filename = malloc (strlen(dest) * sizeof(char));
    strcpy(*filename, dest);
}