float distance_matrix[ MAXNFRAME+1 ][ MAXNFRAME+1 ]; // �洢�������
float distortion_matrix[ MAXNFRAME+1 ][ MAXNFRAME+1 ]; // �洢�����ۼӾ���

int get_strs_from_file( string filename, string strs[] )
{
	ifstream fin( filename.c_str() );
	if( !fin )
	{
		cout << "file read error!" << endl;
		exit(-1);
	}

	int nstr=1;
	while( !fin.eof() )
	{
		getline( fin, strs[nstr++] );
	}
	fin.close();

	while( "" == strs[nstr-1] )nstr--;

	return nstr-1;
}

float CalculateDistance(float Vector1[], float Vector2[]) // Note: ��Ҫ���б�д��
{// ����������ŷʽ����
	float x = 0;
	for (int k = 1; k < DVECTOR + 1; k++)
	{
		x = x + pow(Vector1[k] - Vector2[k], 2);
	}
	return sqrt(x);
}

float Run_DTW( float TemplateV[][DVECTOR+1], int NFTemplate, float TestV[][DVECTOR+1], int NFTest )   // Note: ��Ҫ���б�д��
{
	float distortion = 0.0;
	int i = 0, j = 0;
	for (i = 0; i < NFTemplate+1; i++)
		for (j = 0; j < NFTest + 1; j++)
		{
			if (i == 0 || j == 0)
			{
				distance_matrix[i][j] = 0;
				distortion_matrix[i][j] = 0;
			}
			else
				distance_matrix[i][j] = CalculateDistance(TemplateV[i], TestV[j]);//��������i֡��j֮֡���ŷʽ����
		}
	//��̬�滮������·��
	for (i = 1; i < NFTest + 1; i++)
		distortion_matrix[1][i] = distance_matrix[1][i] + distortion_matrix[1][i - 1];//�����һ����
	for (i = 1; i < NFTemplate + 1; i++)
		distortion_matrix[i][1] = distance_matrix[i][1] + distortion_matrix[i - 1][1];//�����һ����
	for (i = 2; i < NFTemplate+1; i++)
		for (j = 2; j < NFTest + 1; j++)
		{
			float temp;
			temp = distortion_matrix[i - 1][j] < distortion_matrix[i][j - 1]? distortion_matrix[i-1][j] : distortion_matrix[i][j-1];
			temp = temp < distortion_matrix[i - 1][j - 1] ? temp : distortion_matrix[i - 1][j - 1];
			distortion_matrix[i][j] = temp + distance_matrix[i][j];
		}
	distortion = distortion_matrix[NFTemplate][NFTest];
	return distortion; // ���ز������ݺ�ģ������֮����ۼӾ����
}