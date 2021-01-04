#include<NTL/ZZ.h>
#include<time.h>
#include <bitset>
#include<string>
#include<fstream>

#pragma comment(lib,"NTL.lib")

NTL_CLIENT

class MD5 {
private:
#define shift(x, n) (((x) << (n)) | ((x) >> (32-(n))))//���Ƶ�ʱ�򣬸�λһ��Ҫ���㣬�����ǲ������λ
#define F_MD5(x, y, z) (((x) & (y)) | ((~x) & (z)))    
#define G_MD5(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H_MD5(x, y, z) ((x) ^ (y) ^ (z))
#define I_MD5(x, y, z) ((y) ^ ((x) | (~z)))
#define A_MD5 0x67452301
#define B_MD5 0xefcdab89
#define C_MD5 0x98badcfe
#define D_MD5 0x10325476
	ZZ Hex2Dec(char *x) {
		ZZ z;
		z = 0;
		for (int i = 0; i<32; i++) {
			z *= 16;
			if (x[i] <= '9'&&x[i] >= '0') z += x[i] - '0';
			else z += x[i] - 'a' + 10;
		}
		return z;
	}
	//strBaye�ĳ���
	unsigned int strlength;
	//A_MD5,B_MD5,C_MD5,D����ʱ����
	unsigned int atemp;
	unsigned int btemp;
	unsigned int ctemp;
	unsigned int dtemp;
	//����ti unsigned int(abs(sin(i+1))*(2pow32))
	const unsigned int k[64] = {
		0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,
		0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,0x698098d8,
		0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,
		0xa679438e,0x49b40821,0xf61e2562,0xc040b340,0x265e5a51,
		0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
		0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,
		0xfcefa3f8,0x676f02d9,0x8d2a4c8a,0xfffa3942,0x8771f681,
		0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,
		0xbebfbc70,0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,
		0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,0xf4292244,
		0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,
		0xffeff47d,0x85845dd1,0x6fa87e4f,0xfe2ce6e0,0xa3014314,
		0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391 };
	//����λ����
	const unsigned int s[64] = { 7,12,17,22,7,12,17,22,7,12,17,22,7,
		12,17,22,5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
		4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,6,10,
		15,21,6,10,15,21,6,10,15,21,6,10,15,21 };
	const char str16[17] = "0123456789abcdef";
	void mainLoop(unsigned int M[])
	{
		unsigned int f, g;
		unsigned int a = atemp;
		unsigned int b = btemp;
		unsigned int c = ctemp;
		unsigned int d = dtemp;
		for (unsigned int i = 0; i < 64; i++)
		{
			if (i<16) {
				f = F_MD5(b, c, d);
				g = i;
			}
			else if (i<32)
			{
				f = G_MD5(b, c, d);
				g = (5 * i + 1) % 16;
			}
			else if (i<48) {
				f = H_MD5(b, c, d);
				g = (3 * i + 5) % 16;
			}
			else {
				f = I_MD5(b, c, d);
				g = (7 * i) % 16;
			}
			unsigned int tmp = d;
			d = c;
			c = b;
			b = b + shift((a + f + k[i] + M[g]), s[i]);
			a = tmp;
		}
		atemp = a + atemp;
		btemp = b + btemp;
		ctemp = c + ctemp;
		dtemp = d + dtemp;
	}
	/*
	*��亯��
	*�����Ӧ����bits��448(mod512),�ֽھ���bytes��56��mode64)
	*��䷽ʽΪ�ȼ�һ��1,����λ����
	*������64λ��ԭ������
	*/
	unsigned int* add(const char* str, unsigned int lenth)
	{
		unsigned int num = ((lenth + 8) / 64) + 1;//��512λ,64���ֽ�Ϊһ��
		unsigned int *strByte = new unsigned int[num * 16];    //64/4=16,������16������
		strlength = num * 16;
		for (unsigned int i = 0; i < num * 16; i++)
			strByte[i] = 0;
		for (int i = 0; i <(int)lenth; i++)
		{
			strByte[i >> 2] |= (str[i]) << ((i % 4) * 8);//һ�������洢�ĸ��ֽڣ�i>>2��ʾi/4 һ��unsigned int��Ӧ4���ֽڣ�����4���ַ���Ϣ
		}
		strByte[lenth >> 2] |= 0x80 << (((lenth % 4)) * 8);//β�����1 һ��unsigned int����4���ַ���Ϣ,������128����
														   /*
														   *���ԭ���ȣ�����ָλ�ĳ��ȣ�����Ҫ��8��Ȼ����С�������Է��ڵ����ڶ���,���ﳤ��ֻ����32λ
														   */
		strByte[num * 16 - 2] = lenth * 8;
		return strByte;
	}
	void changeHex(int a, char *x)
	{
		int b;
		char str1[4];
		char str[9];
		str[8] = '\0';
		for (int i = 0; i<4; i++)
		{
			str1[3] = '\0';
			b = ((a >> i * 8) % (1 << 8)) & 0xff;   //������ÿ���ֽ�
			for (int j = 0; j < 2; j++)
			{
				char stemp[3];
				stemp[0] = str16[b % 16];
				memcpy(stemp + 1, str1, 1);
				memcpy(str1, stemp, 2);
				b = b / 16;
			}
			memcpy(str + 2 * i, str1, 2);
		}
		memcpy(x, str, 8);
	}
public:
	ZZ getMD5(const char *source, int lenth, char *x)
	{
		atemp = A_MD5;    //��ʼ��
		btemp = B_MD5;
		ctemp = C_MD5;
		dtemp = D_MD5;
		unsigned int *strByte = add(source, lenth);
		for (unsigned int i = 0; i<strlength / 16; i++)
		{
			unsigned int num[16];
			for (unsigned int j = 0; j<16; j++)
				num[j] = strByte[i * 16 + j];
			mainLoop(num);
		}
		char Hex[4][9];
		changeHex(atemp, Hex[0]);
		changeHex(btemp, Hex[1]);
		changeHex(ctemp, Hex[2]);
		changeHex(dtemp, Hex[3]);
		memcpy(x, Hex[0], 8);
		memcpy(x + 8, Hex[1], 8);
		memcpy(x + 16, Hex[2], 8);
		memcpy(x + 24, Hex[3], 8);
		ZZ z;
		z = Hex2Dec(x);
		return z;
	}
};

void RSA_Get_keys(ZZ& pr_key,ZZ& pu_key, ZZ& n) {
	ZZ p, q;
	SetSeed(to_ZZ((unsigned long)time(NULL)));
	GenPrime(p, 512);
	GenPrime(q, 512);
	n = p*q;
	ZZ n_ = (p - 1)*(q - 1);
	RandomBnd(pu_key, SqrRoot(n_));
	while (InvModStatus(pr_key, pu_key, n_) != 0) {
		RandomBnd(pu_key, SqrRoot(n_));
	}
}

ZZ RSA_dencry(ZZ x, const ZZ pr_key, const ZZ n) {
	return PowerMod(x%n, pr_key, n) % n;
}

ZZ RSA_encry(ZZ x, const ZZ pu_key, const ZZ n) {
	return PowerMod(x%n, pu_key, n) % n;
}

typedef bitset<8> byte;
typedef bitset<32> word;

void str2byte(string str,byte out[]) {
	for (int i = 0; i < (int)str.length() / 8; i++)
		for (int j = 7; j >= 0; j--)
			out[i][j] = str[8 * i + 7 - j] - '0';
}

ZZ str2ZZ(string str)
{
	ZZ z;
	z = 0;
	for (int i = str.length() - 1; i >= 0; i--) {
		if (str[i] == '1')z = (z + 1) * 2;
		else if (str[i] == '0')z *= 2;
		else {
			z = -1;
			return z;
		}
	}
	z /= 2;
	return z;
}

string ZZ2str(ZZ z, int length = 128)
{
	string str = "";
	for (int i = 0; i < length; i++) {
		char x = (char)(z % 2) + '0';
		str = str + x;
		z /= 2;
	}
	return str;
}

void byte2char(char out[16], const byte s[16])
{
	for (int i = 0; i < 16; ++i) {
		out[i] = 0;
		for (int j = 7; j > 0; --j)
			out[i] = (out[i] + s[i][j]) * 2;
		out[i] += s[i][0];
	}
}

extern const int Nr = 10;
extern const int Nk = 4;
extern void charToByte(byte out[16], const char s[16]);
extern void GenKey(char* a);
extern int encrypt(string filename, byte key[], word w[]);
extern int decrypt(string filename, word w[], int flag);
extern void KeyExpansion(byte key[4 * Nk], word w[4 * (Nr + 1)]);

int EN_file(const ZZ pr_key, const ZZ pu_key, const ZZ n, string filename)
{
	fstream in;
	word w[4 * (Nr + 1)];
	byte key[16];
	if (encrypt(filename, key, w) == -1)
		return -1;
	cout << "\n���ܳɹ�" << endl;
	string str = "";
	for (int i = 0; i < 16; i++) {
		str += key[i].to_string();
	}
	ZZ z, T;
	if ((z = str2ZZ(str)) == -1) {
		cout << "��Կ�����д�" << endl;
		return -1;
	}
	T = RSA_encry(z, pu_key, n);
	ZZ temp_T = T;
	string temp = "";
	for (int i = 0; temp_T != 0; i++) {
		char x = (char)(temp_T % 10 + '0');
		temp = x + temp;
		temp_T /= 10;
	}
	MD5 md5;
	char xx[33];
	ZZ T_ = md5.getMD5(temp.c_str(), temp.length(), xx);
	T_ = RSA_dencry(T_, pr_key, n);
	in.open("KEY\\" + filename + ".key", ios::out);
	if (!in.is_open()) {
		cout << "��Կ�ļ�����ʧ��" << endl;
		return -1;
	}
	in << T << endl;
	in << T_ << endl;
	in.close();
	cout << "\n��Կ���ܱ������(λ��KEY�ļ����ڣ��Ͻ��޸�)" << endl;
	return 1;
}

int DE_file(const ZZ pr_key, const ZZ pu_key, const ZZ n, string filename, string ID, int flag = 1)
{
	fstream in;
	word w[4 * (Nr + 1)];
	byte key[16];
	ZZ t, T, T_1,T_2;
	string temp = "KEY\\" + filename + ".key";
	in.open(temp, ios::in);
	if (!in.is_open()) {
		cout << "��Կ�ļ�ȱʧ" << endl;
		return -1;
	}
	in >> T >> T_1;
	ZZ temp_T;
	temp_T = T;
	in.close();
	string temp_ = "";
	for (int i = 0; temp_T != 0; i++) {
		char x = (char)(temp_T % 10 + '0');
		temp_ = x + temp_;
		temp_T /= 10;
	}
	MD5 md5;
	char xx[33];
	T_2 = md5.getMD5(temp_.c_str(), temp_.length(), xx);
	T_1 = RSA_encry(T_1, pu_key, n);
	if (T_2 != T_1) {
		cout << filename << "�ļ�������" << ID << "���ܵ�" << endl;
		return -1;
	}
	remove(temp.c_str());
	t = RSA_dencry(T, pr_key, n);
	string str = ZZ2str(t);
	str2byte(str, key);
	KeyExpansion(key, w);
	if (decrypt(filename, w, flag) == -1)
		return -1;

	//char temp[17] = "";
	//byte2char(temp, key_);
	return 1;
}

int Get_ID(string &ID) 
{
	ifstream in;
	int time = 3;
	while (time--) {
		ID.clear();
		cout << "\n������ID����������RSAԿ��" << endl;
		cin >> ID;
		in.open("Public_KEY\\" + ID, ios::in);
		if (in.is_open()) {
			in.close();
			in.open("Private_KEY\\" + ID, ios::in);
			if (in.is_open()) {
				in.close();
				break;
			}
			cout << "��ID��˽Կ������,����������" << endl;
			continue;
		}
		cout << "��ID��˽Կ������,����������" << endl;
	}
	if (time == -1) {
		cout << "ʧ�ܹ���" << endl;
		return -1;
	}
	return 1;
}

void Get_KEY(string ID, ZZ& pr_key, ZZ& pu_key, ZZ& n)
{
	ifstream in;
	in.open("Public_KEY\\" + ID, ios::in);
	in >> pu_key >> n;
	in.close();
	in.open("Private_KEY\\" + ID, ios::in);
	in >> pr_key;
	in.close();
}

int main()
{
	fstream in;
	string md = "@echo off\nmd KEY 2>nul\nmd Public_KEY 2>nul\nmd Private_KEY 2>nul\n";
	in.open("md.bat", ios::out);
	in << md;
	in.close();
	system("md.bat");
	remove("md.bat");
	while (1) {
		system("cls");
		cout << "�����¹��ܣ�" << endl;
		cout << "a:���ɹ�˽Կ" << endl;
		cout << "b:�����ļ�" << endl;
		cout << "c:�����ļ�" << endl;
		cout << "d:�����ļ���" << endl;
		cout << "e:�����ļ���" << endl;
		cout << "�����룺";
		char flag;
		cin >> flag;
		while ((flag < 'a'||flag > 'e')&&(flag < 'A'||flag > 'E')) {
			cout << "\n������������������:";
			cin.ignore(1024, '\n');
			cin.clear();
			cin.sync();
			cin >> flag;
		}
		ZZ pr_key, pu_key, n;
		string ID;
		if (flag == 'a') {
			int time = 3;
			while (time--) {
				ID.clear();
				cout << "������ID����������RSAԿ��" << endl;
				cin >> ID;
				in.open("Public_KEY\\" + ID, ios::in);
				if (in.is_open()) {
					cout << "��ID�Ѵ���,����������" << endl;
					in.close();
					continue;
				}
				in.open("Private_KEY\\" + ID, ios::in);
				if (in.is_open()) {
					cout << "��ID�Ѵ���,����������" << endl;
					in.close();
					continue;
				}
				break;
			}
			if (time == -1) {
				cout << "ʧ�ܹ���" << endl;
				continue;
			}
			RSA_Get_keys(pr_key, pu_key, n);
			in.open("Public_KEY\\" + ID, ios::out);
			in << pu_key << endl;
			in << n;
			in.close();
			in.open("Private_KEY\\" + ID, ios::out);
			in << pr_key;
			in.close();
			cout << "���ɹ�˽Կ���,�뱣�ܺ�˽Կ\n" << endl;
		}
		else if (flag == 'b') {
			string filename;
			cout << "ִ�м��ܹ���֮ǰ���뽫˽Կ�ļ�����Private_KEY�ļ����ڣ������ܳɹ�" << endl;
			if (Get_ID(ID) == -1) {
				continue;
			}
			Get_KEY(ID, pr_key, pu_key, n);
			cout << "\n������������ļ���(����׺):";
			cin >> filename;
			if (EN_file(pr_key, pu_key, n, filename) == 1)
				cout << "�����ļ����,Ϊ" << filename + ".cip,������ɾ��ԭ�ļ�\n" << endl;
		}
		else if (flag == 'c') {
			string filename;
			cout << "ִ�н��ܹ���֮ǰ���뽫˽Կ�ļ�����Private_KEY�ļ����ڣ������ܳɹ�" << endl;
			if (Get_ID(ID) == -1) {
				continue;
			}
			Get_KEY(ID, pr_key, pu_key, n);
			cout << "\n������������ļ���(����׺):";
			cin >> filename;
			if (DE_file(pr_key, pu_key, n, filename, ID) == 1)
				cout << "�����ļ����,Ϊ" << filename + ".data,������ȥ����׺" << "\n" << endl;
		}
		else if (flag == 'd') {
			string filename;
			if (Get_ID(ID) == -1) {
				continue;
			}
			string comm = "\"WinRar\\WinRAR.exe\" m -r -or -ep1";
			cout << "\n������Ҫ���ܵ��ļ�����" << endl;
			cin >> filename;
			comm += ' ' + filename + ".rar " + filename;
			in.open("rar.bat", ios::out);
			in << "@echo off" << endl;
			in << "cd /d %~dp0" << endl;
			in << comm << endl;
			comm = "ren " + filename + ".rar " + filename;
			in << comm << endl;
			in.close();
			system("rar.bat");
			remove("rar.bat");
			Get_KEY(ID, pr_key, pu_key, n);
			if (EN_file(pr_key, pu_key, n, filename) == 1)
				cout << "\n�����ļ������(Ϊ��֤��ɣ����Ժ����ٲ���)\n" << endl;
			remove(filename.c_str());
			comm = "ren " + filename + ".cip " + filename;
			in.open("ren.bat", ios::out);
			in << "@echo off" << endl;
			in << comm << endl;
			in.close();
			system("ren.bat");
			remove("ren.bat");
		}
		else if (flag == 'e') {
			string filename;
			if (Get_ID(ID) == -1) {
				continue;
			}
			string comm;
			cout << "\n������Ҫ���ܵ��ļ�����" << endl;
			cin >> filename;
			Get_KEY(ID, pr_key, pu_key, n);
			if (DE_file(pr_key, pu_key, n, filename, ID, 2) == 1) {
				cout << "\n�����ļ������(Ϊ��֤��ɣ����Ժ����ٲ���)\n" << endl;
				remove(filename.c_str());
				comm = "ren " + filename + ".data " + filename + ".rar";
				in.open("ren.bat", ios::out);
				in << "@echo off" << endl;
				in << comm << endl;
				comm = "\"WinRar\\WinRAR.exe\" x -Y -c- " + filename + ".rar";
				in << "cd /d %~dp0" << endl;
				in << comm << endl;
				in.close();
				system("ren.bat");
				remove("ren.bat");
				filename += ".rar";
				remove(filename.c_str());
			}
		}

		cout << "\n����e���������������:";
		string x;
		cin >> x;
		if (x == "e")break;
	}

	return 0;
}