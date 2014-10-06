#include <pgm.h>
#include <stdio.h> 

#define MID_ROW 3
#define MID_COL 3

double convolmatrix[7][7] = {0.00000067,  0.00002292,  0.00019117,  0.00038771,  0.00019117,  0.00002292,  0.00000067,
                             0.00002292,  0.00078633,  0.00655965,  0.01330373,  0.00655965,  0.00078633,  0.00002292,
                             0.00019117,  0.00655965,  0.05472157,  0.11098164,  0.05472157,  0.00655965,  0.00019117,
			     0.00038771,  0.01330373,  0.11098164,  0.22508352,  0.11098164,  0.01330373,  0.00038771,
			     0.00019117,  0.00655965,  0.05472157,  0.11098164,  0.05472157,  0.00655965,  0.00019117,
			     0.00002292,  0.00078633,  0.00655965,  0.01330373,  0.00655965,  0.00078633,  0.00002292,	
			     0.00000067,  0.00002292,  0.00019117,  0.00038771,  0.00019117,  0.00002292,  0.00000067,	
			    };
	
void blur(FILE *infile, FILE *outfile)
{
	int n_rows, n_cols, maxval;
	gray **allocarray;

	allocarray = pgm_readpgm(infile, &n_cols, &n_rows, &maxval);

	int i, j;
	int up_row_pgm, down_row_pgm, left_col_pgm, right_col_pgm;
	int up_row_matrix, down_row_matrix, left_col_matrix, right_col_matrix;

	for(i = 0; i < n_rows; i++)
	{	
		for(j = 0; j < n_cols; j++)
		{
			if(i < MID_ROW)
			{
				up_row_pgm = 0;
				up_row_matrix = MID_ROW - i; 
			}
			else
			{
				up_row_pgm = i - MID_ROW;
				up_row_matrix = 0;
			}
			
			if(j < MID_COL)
			{
				left_col_pgm = 0;	
				left_col_matrix = MID_COL - j;
			}
			else
			{
				left_col_pgm = j - MID_COL;
				left_col_matrix = 0;
			}

			if(i > n_rows - 1 - MID_ROW)
			{
				down_row_pgm = n_rows - 1;
				down_row_matrix = MID_ROW + n_rows - 1 - i;
			}
			else
			{
				down_row_pgm = i + MID_ROW;
				down_row_matrix = 6;
			}

			if(j > n_cols - 1 - MID_COL)
			{
				right_col_pgm = n_cols - 1;
				right_col_matrix = MID_COL + n_cols - 1 - j;
			}
			else
			{
				right_col_pgm = j + MID_COL;
				right_col_matrix = 6;	
			}

			int k, l, m, n;
			double sum_weighted_pixel = 0;
			double sum_weight = 0;
			
			for(k = up_row_pgm, l = up_row_matrix; k <= down_row_pgm; k++, l++)
			{
				for(m = left_col_pgm, n = left_col_matrix; m <= right_col_pgm; m++, n++)
				{
					sum_weighted_pixel = sum_weighted_pixel + ((double)allocarray[k][m]) * convolmatrix[l][n];
					sum_weight = sum_weight + convolmatrix[l][n];
				}
			}
			
			allocarray[i][j] = (gray)((int)(sum_weighted_pixel/sum_weight + 0.5)) ;

			if(allocarray[i][j] > maxval)
			{
				allocarray[i][j] = (gray)maxval;
			}
		}	
	}

	int forceplain = 0;

	pgm_writepgm(outfile, allocarray, n_cols, n_rows, maxval, forceplain);

	/*for(i = 0; i < n_rows; i++)
	{
		pgm_freerow(allocarray[i]);
	}*/
	pgm_freearray(allocarray, n_rows);

	

}

int main(int argc, char *argv[])
{	
	if(argc < 3)
	{
		printf("Lack of command line arguments");
		return 0;
	}

	char *infile;
	char *outfile;
	FILE *in;
        FILE *out;
	
	infile = argv[1];
	outfile = argv[2];
	
	in = fopen(infile, "r");
        out = fopen(outfile, "w");
	
	blur(in, out);
	
	fclose(in);
	fclose(out);	
}

