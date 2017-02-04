#include <stdio.h>

int main(int argc, char** argv)
{
  //checking if a file was inputted in the command line
  if(argc<2)
  {
    printf("No file was inputted");
    return 1;
  }
  //taking the file from the command line and openingi it as a binary file
  FILE* f = fopen(argv[1], "rb");
  if(f == NULL)
	{
		printf("That file doesn't exist!\n");
		return 1;
	}
    //Struct used to read in exif tags
    struct exifTag{
      short fileStart;
      short APP1;
      short app1BlockEnd;
      char exifString[4];
      short NULTerm;
      char endian[2];
      short fourtyTwo;
    };

    //will be important soon
    unsigned short count;

    //struct for reading in tiff tags
    struct tiffTag{
      short tagID;
      short dataType;
      int numOfItems;
      int offset;
    };

    //each of these are specific tiff tags to find in the file
    struct exifTag hankHill;
    struct tiffTag ManString;
    struct tiffTag CamModString;
    struct tiffTag exifSBA;
    struct tiffTag expSpeed;
    struct tiffTag fStop;
    struct tiffTag lensLength;
    struct tiffTag dateTaken;

    //initial reading of the exif tag
    fread(&hankHill, sizeof(hankHill), 1, f);

    //checking if there is an APP0 in the file, if so this cannot run
    if(hankHill.APP1 == 0xe0ff)
    {
      printf("Invalid file, sorry!");
      return 1;
    }

    //checking if the current endianess is supported, if no, this cannot run
    else if(hankHill.endian[0] == 'M')
    {
      printf("That endianness is not supported, sorry!");
      return 1;
    }


    //reading in count in order to loop the file and find specific tiff tags with
    //the info we want
    fseek(f, 20, SEEK_SET);
    fread(&count, sizeof(count), 1, f);
    struct tiffTag tagArray[count];
    int i;
    for(i=0; i<count; i++)
    {
      fread(&tagArray[i], sizeof(tagArray[i]), 1, f);

      //This specific tag contains the manufacturer
      if(tagArray[i].tagID == 0x010f)
      {
        ManString = tagArray[i];
      }

      //This specific tag contains the camera model
      else if(tagArray[i].tagID == 0x0110)
      {
        CamModString = tagArray[i];
      }

      //This specific tag is useful for finding more data fields
      else if(tagArray[i].tagID == 0xffff8769)
      {
        exifSBA = tagArray[i];
        break;
      }
    }

    //Getting the manufacturer
    char asciiString[ManString.numOfItems];
    fseek(f, (ManString.offset+12), SEEK_SET);
    fread(&asciiString, sizeof(asciiString), 1, f);
    printf("Manufacturer:\t%s\n", asciiString);

    //getting the camera model
    char asciiString2[CamModString.numOfItems];
    fseek(f, (CamModString.offset+12), SEEK_SET);
    fread(&asciiString2, sizeof(asciiString2), 1, f);
    printf("Camera Model:\t%s\n", asciiString2);

    //reading in a new count that will help find the last few data fields
    fseek(f, (exifSBA.offset+12), SEEK_SET);
    count=0;
    fread(&count, sizeof(count), 1, f);
    struct tiffTag tiff2[count];
    int j;
    for(j=0; j<count; j++)
    {
      fread(&tiff2[i], sizeof(tiff2[i]), 1, f);

      //This specific tag contains the width in pixels
      if(tiff2[i].tagID == 0xffffA002)
      {
        printf("Width:\t\t%d pixels\n", tiff2[i].offset);
      }

      //This specific tag contains the height in pixels
      else if(tiff2[i].tagID == 0xffffA003)
      {
        printf("Height:\t\t%d pixels\n", tiff2[i].offset);
      }

      //This specific tag contains ISO
      else if(tiff2[i].tagID == 0xffff8827)
      {
        printf("ISO:\t\tISO %d\n", tiff2[i].offset);
      }

      //This tags all contain more meta data, but must be handled differently
      //outside of the loop
      else if(tiff2[i].tagID == 0xffff829a)
      {
        expSpeed = tiff2[i];
      }
      else if(tiff2[i].tagID == 0xffff829d)
      {
        fStop = tiff2[i];
      }
      else if(tiff2[i].tagID == 0xffff920a)
      {
        lensLength = tiff2[i];
      }
      else if(tiff2[i].tagID == 0xffff9003)
      {
        dateTaken = tiff2[i];
      }
    }

    //Printing the exposure speed
    unsigned int expSpeed1, expSpeed2;
    fseek(f, (expSpeed.offset+12), SEEK_SET);
    fread(&expSpeed1, sizeof(expSpeed1), 1, f);
    fread(&expSpeed2, sizeof(expSpeed2), 1, f);
    printf("Exposure Speed:\t%d/%d second\n", expSpeed1, expSpeed2);

    //Printing the F-Stop
    unsigned int fStop1, fStop2;
    fseek(f, (fStop.offset+12), SEEK_SET);
    fread(&fStop1, sizeof(fStop1), 1, f);
    fread(&fStop2, sizeof(fStop2), 1, f);
    printf("F-Stop:\t\tf/%f\n", ((double)fStop1/fStop2));

    //Printing the lens length
    unsigned int lens1, lens2;
    fseek(f, (lensLength.offset+12), SEEK_SET);
    fread(&lens1, sizeof(lens1), 1, f);
    fread(&lens2, sizeof(lens2), 1, f);
    printf("Focal Length:\t%d mm\n", (lens1/lens2));

    //Printing the date the picture was taken
    char date[dateTaken.numOfItems];
    fseek(f, (dateTaken.offset+12), SEEK_SET);
    fread(&date, sizeof(date), 1, f);
    printf("Date Taken:\t%s\n", date);

    fclose(f);
    return 0;
}
