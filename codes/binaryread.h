// notice: the file name must be "InFile"
// this file is used to read data from a binary file such as mfcc-file.

ifstream InFile;

short ReadShort()
{
	int data = 0;
	char *cache = new char [2];
	unsigned char* machine = (unsigned char*)&data; // 机器数,4字节
	for( int i=1; i>=0; i-- )
	{
		InFile.read( &cache[i], 1 );
	}
	for( int i=0; i<2; i++ )
	{
		machine[i] = ( unsigned char )cache[i]; // big-endian ordering in UNIX while little-endian ordering in Windows
	}
	delete cache;
	return data;
}

int ReadInt()
{
	int data = 0;
	char *cache = new char [4];
	unsigned char* machine = (unsigned char*)&data; // 机器数,4字节
	for( int i=3; i>=0; i-- )
	{
		InFile.read( &cache[i], 1 );
	}
	for( int i=0; i<4; i++ )
	{
		machine[i] = ( unsigned char )cache[i]; // big-endian ordering in UNIX while little-endian ordering in Windows
	}
	delete cache;
	return data;
}

float ReadFloat()
{
	float data = 0.0f;
	char *cache = new char [4];
	unsigned char *machine = ( unsigned char* )&data; // machine 中存储机器码，单精度32位数值,4字节
	for( int i=3; i>=0; i-- )
	{
		InFile.read( &cache[i], 1 );	
	}
	for( int i=0; i<4; i++ )
	{
		machine[i] = ( unsigned char )cache[i]; // big-endian ordering in UNIX while little-endian ordering in Windows
	}
	delete cache;
	return data;
}

int ReadHtk( string InputFile, float FrameV[][DVECTOR+1])
{
	//readin mfc data
	InFile.open( InputFile.c_str(), ios::binary ); 
	if( !InFile )
	{	cout << "no mfc file exist!"; exit( -1 ); }
	long nf = 0,fp = 0;
	short by = 0, tc = 0;
		
	nf = ReadInt(); // number of frames
	fp = ReadInt(); // frame interval (unit: 100 ns)
	by = ReadShort(); // bytes per frame
	tc = ReadShort(); // type code 
	for( int i=1; i<=nf; i++ )
	{	
		for( int k=1; k<=DVECTOR; k++ )
		{
			FrameV[i][k] = ReadFloat();
		}
	}
	InFile.close();

	return nf;
}