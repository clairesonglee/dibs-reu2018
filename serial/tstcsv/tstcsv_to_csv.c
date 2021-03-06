/**
 * Transforms the ECML PKDD csv file format into a generalized GPS trajectory
 * csv file format of "ID,LATTITUDE,LONGITUDE". The input and output filepaths
 * are defined as constants. All GPS coordinates within the input file will be
 * included wihtin the exported generalized-format csv file.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

//#define INPUT_FILEPATH "../input/Porto_taxi_data_test_partial_trajectories.csv"
//#define OUTPUT_FILEPATH "../output/Porto_taxi_data_test_partial_trajectories_transformed.csv"
#define LINE_OFFSET 1
#define MAX_INPUT_LINE_SIZE 20000
#define MAX_OUTPUT_LINE_SIZE 64
//#define NUM_COORDINATES 14438		//For Cheaters only
#define NUM_FIELDS 9

// Loads input file at the provided filepath into the provided buffer
// Returns the size of the input file
size_t load_csv_to_buffer(char *filepath, char **buffer) {
	FILE *file = fopen(filepath, "r");
	if (file == NULL) {
		printf("Cannot open file \n");
		return 0;	
	}

	// find the length of the file
	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	rewind(file);

	*buffer = (char *)malloc(sizeof(char) * fileSize + 1);

	fread(*buffer, fileSize, 1, file);
	fclose(file);
	printf("File successfully read \n");

	return fileSize;
}


// Appends the id and coordinates within a provided token to the output bufffer
// Returns the size of the appended line.
size_t input_coordinates(char *token, char **marker) {
    char *tokenDelimiters = ",[]\" ";
    char *lattitude;
    char *longitude;
    char *id;

    // Read in useful data
    id = strtok(token, tokenDelimiters);

    int i;
    for (i = 0; i < NUM_FIELDS - 2; ++i) {
        strtok(NULL, tokenDelimiters);
    }
    // Flip order of the coordinate to match (lattitude, longitude) order
    longitude = strtok(NULL, tokenDelimiters);
    lattitude = strtok(NULL, tokenDelimiters);

    //printf("%s,%s,%s\n", id, lattitude, longitude);

    size_t size = 0;
    while(lattitude != NULL && longitude != NULL) {
        memcpy(*marker, id, strlen(id) * sizeof(char));
        *marker += strlen(id) * sizeof(char);
        memcpy(*marker, ",", sizeof(char));
        *marker += sizeof(char);
        memcpy(*marker, lattitude, strlen(lattitude) * sizeof(char));
        *marker += strlen(lattitude) * sizeof(char);
        memcpy(*marker, ",", sizeof(char));
        *marker += sizeof(char);
        memcpy(*marker, longitude, strlen(longitude) * sizeof(char));
        *marker += strlen(longitude) * sizeof(char);
        memcpy(*marker, "\n", sizeof(char));
        *marker+= sizeof(char);

        size += (strlen(id) + strlen(lattitude) + strlen(longitude) + 3) *
            sizeof(char);

        longitude = strtok(NULL, tokenDelimiters);
        if (longitude != NULL) {
            lattitude = strtok(NULL, tokenDelimiters);
        }
    }
    return size;
}


// Converts the provided input file buffer into a generalized csv format and
// writes it into the provided output buffer
// Returns the size of the output buffer
size_t convert_to_csv(char *inputBuffer, char **outputBuffer) {
    //printf("In convert_to_csv\n");
    char *fileDelimiters = "\n";
    char *token = malloc(MAX_INPUT_LINE_SIZE * sizeof(char));

    token = strtok(inputBuffer, fileDelimiters);
    char *marker = *outputBuffer;

    // Ignore headers/unimportant data
    int i;
    for (i = 0; i < LINE_OFFSET; ++i) {
        inputBuffer += strlen(token) + 1;
        token = strtok(inputBuffer, fileDelimiters);
    }

    size_t fileSize = 0;
    while (token != NULL) {
        inputBuffer += strlen(token) + 1;
        fileSize += input_coordinates(token, &marker);
        token = strtok(inputBuffer, fileDelimiters);
    }
    free(token);
    return fileSize;
}


// Writes file of provided file size with contents of provided buffer
int write_csv_file(char* filepath, char* buffer, size_t fileSize) {
    FILE *file = fopen(filepath, "w");
	if(file ==  NULL){
		return -1;
	}
    fwrite(buffer, fileSize, sizeof(char), file);
    fclose(file);
	return 0;
}


int main(int argc, char *argv[]) { 
    double start_time, end_time, trans_start, trans_end;
    char *inputBuffer;
    char *outputBuffer;
	if(argc != 3){
		printf("Incorrect executable call:\n");
		printf("\tUsage: ./bin inputfile outputfile\n");
		return -1;
	}
    start_time = omp_get_wtime();
    size_t inputSize = load_csv_to_buffer(argv[1], &inputBuffer);
	if(inputSize == 0){
		return -1;
	}   
	outputBuffer = malloc(inputSize * sizeof(char));

    // convert the csv file
    trans_start = omp_get_wtime();
    size_t outputSize = convert_to_csv(inputBuffer, &outputBuffer);
    trans_end = omp_get_wtime() - trans_start;

    if(write_csv_file(argv[2], outputBuffer,outputSize)){
		printf("Error writing output file\n");		
		return -1;
	}
    end_time = omp_get_wtime() - start_time;
    printf("%s, %g, %g", argv[2], end_time, trans_end);
    //printf("%s", outputBuffer);

    free(inputBuffer);
    free(outputBuffer);
	return 0;
}
