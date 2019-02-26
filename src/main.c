#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

struct InputFileDetails {
    char *name;
    int size;
    char *buffer;
} inputFileDetails;

void writeRawFile(char *fname, int sectorCount, struct InputFileDetails *inputFileDetails);
void readInputFile(struct InputFileDetails *inputFileDetails);
void displayHelp();

int main(int argc, char *argv[]) {
    if (argc == 1) {
        displayHelp();
        return 1;
    }

    int sectorCount = 2880;
    char *filename = NULL;

    int opt;

    printf("\nhoss-disk-builder v1.0\n");

    while((opt = getopt(argc, argv, ":i:s:")) != -1) {  
        switch(opt)  
        {  
            case 'i':  
                inputFileDetails.name = optarg;
                break;  
            case 's':
                sectorCount = atoi(optarg);
                if (sectorCount == 0) {
                    printf("Invalid sector count\n");
                    return 1;
                }
                break;
            case ':':  
                printf("option needs a value\n");  
                break;  
            case '?':  
                printf("unknown option: %c\n", optopt); 
                break;  
        }  
    }  
      
    // optind is for the extra arguments 
    // which are not parsed 
    int fileInd = 0;
    for(; optind < argc; optind++){
        if (fileInd == 1) {
            printf("Too many parameters\n");
            displayHelp();
            return 1;
        }
        filename = argv[optind];
        fileInd = 1;
    } 

    if (!filename) {
        printf("Missing output filename\n");
        displayHelp();
        return 1;
    }

    printf("  Sector size: %i\n", 512);
    printf("  Sector count: %i\n", sectorCount);

    readInputFile(&inputFileDetails);
    if (inputFileDetails.size == -1) {
        return 1;
    } else if (!inputFileDetails.buffer) {
        printf("Unable to read input file %s\n", inputFileDetails.name);
        return 1;
    }
    writeRawFile(filename, sectorCount, &inputFileDetails);

    free(inputFileDetails.buffer);
    printf("\n");
    return 0;
}

void writeRawFile(char *fname, int sectorCount, struct InputFileDetails *inputFileDetails) {
    printf("Creating file '%s'\n", fname);

    char *buff = malloc(512);
    for (int i = 0; i < 512; i++) {
        buff[i] = 0;
    }

    FILE *file;
    if ((file = fopen(fname, "wb"))) {
        // First write input file
        if (inputFileDetails->size > 0) {
            fwrite(inputFileDetails->buffer, sizeof(char), inputFileDetails->size, file);
        }

        int sectorsOccupied = (inputFileDetails->size / 512) + 1;
        int bytesRemainingInCurrent = (sectorsOccupied * 512) - inputFileDetails->size;
        fwrite(buff, sizeof(*buff), bytesRemainingInCurrent, file); 

        for (int i = sectorsOccupied; i < sectorCount; i++) {
            fwrite(buff, sizeof(*buff), 512, file);
        }

        int size = ftell(file);
        printf("  Finale size: %i\n", size);

        fclose(file);
    } else {
        printf("Error: Unable to create file %s\n", fname);
    }

    free(buff);
    return;
}

void readInputFile(struct InputFileDetails *inputFileDetails) {
    inputFileDetails->buffer = NULL;
    FILE *file;
    if ((file = fopen(inputFileDetails->name, "rb"))) {
        fseek(file, 0L, SEEK_END);
        inputFileDetails->size = ftell(file);
        rewind(file);
        printf("  Input file: %s\n", inputFileDetails->name);
        printf("    Size: %i\n", inputFileDetails->size);

        inputFileDetails->buffer = malloc(inputFileDetails->size);
        fread(inputFileDetails->buffer, sizeof(char), inputFileDetails->size, file);

        fclose(file);
    } else {
        inputFileDetails->size = -1;
        printf("Error: Unable to read input file %s\n", inputFileDetails->name);
    }
}

void displayHelp() {
    printf("Usage: hdb -ifs <file>\n");
}