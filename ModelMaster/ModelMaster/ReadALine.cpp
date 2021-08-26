#include "stdafx.h"
#include "ReadALine.h"
char *TReadALine::ReadAString(FILE *stream)
{
	if(fgets(string,LINELEN,stream)==NULL)
	{
		endfile=TRUE;
		line=0;
		return "";
	}
	line++;
	return string;
};
#define TAB 9
BOOL TReadALine::ReadInteger(FILE *stream,int start,int num_of_int,
 int *intarray)
{
	return ReadIntegers(stream,start,num_of_int,intarray);
}
BOOL TReadALine::ReadIntegers(FILE *stream,int start,int &num_of_int,int *intarray)
{
	int i,k=0,kk=0,length;
	char s[9],key,str[MAX_PATH];
	BOOL flag;
	if(fgets(string,LINELEN,stream)==NULL)
	{
		endfile=TRUE;
		line=0;
		num_of_int=0;
		return TRUE;
	}
	line++;
	if(num_of_int==0) return TRUE;
	length=strlen(string);
	flag=TRUE;
	for(i=start;i<length;i++)
	{
		if(*(string+i)!=' ')
		{
			flag=FALSE;
			break;
		}
	}
	if((length==0)||(flag==TRUE)) // to end of file for NULL line
	{
		endfile=TRUE;
		line=0;
		num_of_int=0;
		return TRUE;
	}
	for(i=start;i<length;i++)
	{
		key=*(string+i);
		if(((key>='0')&&(key<='9'))||(key=='+')||(key=='-'))
		{
			s[k]=key;
			k++;
			if(k>=7)
			{
				sprintf(str,"line %d in %s",line,FileName);
				////MessageBox(AfxGetMainWnd()->GetSafeHwnd(),"int Str is too long",
				////	str,MB_OK | MB_ICONEXCLAMATION);
				return FALSE;
			}
		}
		else if((key==' ')||(key==',')||(key==TAB)||(key=='\n'))
		{
			if(k==0) continue;
			else
			{
				s[k]=0;
				*(intarray+kk)=atoi(s);
				kk++;
				if(kk==num_of_int) return TRUE;
				k=0;
			}
		}
		else if(key=='/')  //后面可以跟以'/'开头的注释
		{
			if(k!=0)
			{
				s[k]=0;
				*(intarray+kk)=atoi(s);
				kk++;
				if(kk==num_of_int) return TRUE;
				k=0;
			}
			break;
		}
		else
		{
			sprintf(str,"%d char in line %d in %s",i,line,FileName);
			////MessageBox(AfxGetMainWnd()->GetSafeHwnd(),"is unrecognized for int",
			////	str,MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}
	}
	num_of_int=kk;
	return TRUE;
}

BOOL TReadALine::ReadDouble(FILE *stream,int start,int num_of_dble,float *dblearray)
{
	return ReadDoubles(stream,start,num_of_dble,dblearray);
}
BOOL TReadALine::ReadDoubles(FILE *stream,int start,int &num_of_dble,float *dblearray)
{
	int i,k=0,kk=0,length;
	char s[30],key,str[MAX_PATH];
	BOOL flag;
	if(fgets(string,LINELEN,stream)==NULL)
	{
		endfile=TRUE;
		num_of_dble=0;
		return TRUE;
	}
	line++;
	if(num_of_dble==0) return TRUE;
	length=strlen(string);
	flag=TRUE;
	for(i=start;i<length;i++)
	{
		if(*(string+i)!=' ')
		{
			flag=FALSE;
			break;
		}
	}
	if((length==0)||(flag==TRUE)) // to end of file for NULL line
	{
		endfile=TRUE;
		line=0;
		num_of_dble=0;
		return TRUE;
	}
	for(i=start;i<length;i++)
	{
		key=*(string+i);
		if(((key>='0')&&(key<='9'))||(key=='.')||(key=='+')
			||(key=='-')||(key=='e')||(key=='E'))
		{
			s[k]=key;
			k++;
			if(k>=30)
			{
				sprintf(str,"Line %d in %s",line,FileName);
				////MessageBox(AfxGetMainWnd()->GetSafeHwnd(),"double Str is too long",
				////	str,MB_OK | MB_ICONEXCLAMATION);
				return FALSE;
			}
		}
		else if((key==' ')||(key==',')||(key==TAB)||(key=='\n'))
		{
			if(k==0) continue;
			else
			{
				s[k]=0;
				*(dblearray+kk)=float(atof(s));
				kk++;
				if(kk==num_of_dble) return TRUE;
				k=0;
			}
		}
		else if(key=='/') //后面可以跟以'/'开头的注释
		{
			if(k!=0)
			{
				s[k]=0;
				*(dblearray+kk)=float(atof(s));
				kk++;
				if(kk==num_of_dble) return TRUE;
				k=0;
			}
			break;
		}
		else
		{
			sprintf(str,"%d char in line %d in %s",i,line,FileName);
				////MessageBox(AfxGetMainWnd()->GetSafeHwnd(),"unrecognized for double",
				////str,MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}
	}
	num_of_dble=kk;
	return TRUE;
}
void TReadALine::SetFileName(const char *name)
{
  strcpy(FileName,name);
  line=0;
  endfile=FALSE;
};

BOOL TReadALine::GetNumFromStr(const char *str1,const char *str2,char start,int num,float *ff)
{
  char *p,*str,s[31],key;
  int i,k,i2,kk;
  char *pchStr1=(char *)str1;
  str=&(*(pchStr1+start));
  if((p=strstr(str,str2))!=NULL)
    {
    k=0;
    kk=0;
	 p=p+strlen(str2);
	 i2=strlen(p)+1;
    for(i=0;i<i2;i++)
      {
      key=*(p+i);
      if(((key>='0')&&(key<='9'))||(key=='.')||(key=='+')
        ||(key=='-')||(key=='e')||(key=='E'))
        {
        s[k]=key;
        k++;
        if(k>=30)
	  {
//	  sprintf(str,"Line %ld in %s",line,FileName);
//	  MessageBox(States->ParentWin,"A string for a double is too long!",
//		 str,MB_OK | MB_ICONEXCLAMATION);
	  return FALSE;
	  }
        }
      else
//       if((key==' ')||(key==',')||(key==TAB)||(key=='\n'))
        {
        if(k==0) continue;
        else
	  {
	  s[k]=0;
	  ff[kk]=float(atof(s));
	  kk++;
			 k=0;
          if(kk>=num) return TRUE;
	  }
	}
/*      else
        {
        sprintf(str,"%d char in line %d in %s",i,line,FileName);
        MessageBox(States->ParentWin,"is unrecognized for double!",
	  str,MB_OK | MB_ICONEXCLAMATION);
	return FALSE;
	} */
      }
    }
 // sprintf(str,"Line %ld in %s",line,FileName);
 // MessageBox(States->ParentWin,"No enough float to read!",
 //   str,MB_OK | MB_ICONEXCLAMATION);
  return FALSE;
};

using namespace std;

//按照空格打断读取字符
BOOL TReadALine::GetStrFromStr(const char *str1, char *s)
{
	char *aa;
	//char *s;
	char a;
	char *pchStr1 = (char *)str1;
	a = (*(pchStr1));
	char b = ' ';
	while (a == ' ')
	{
		pchStr1++;
		a = (*pchStr1);
	}
	int i = 0;
	while ((a != ' ')&&( a != '\n'))
	{
		s[i] = a;
		i++;
		pchStr1++;
		a = (*pchStr1);
	}
	s[i] = '\0';
	return TRUE;	
}

BOOL TReadALine::GetStrsFromStr(const char *str1,char s[][20])
{

	char a;
	char *pchStr1 = (char *)str1;
	a = (*(pchStr1));
	char b = ' ';
	while (a == ' ')
	{
		pchStr1++;
		a = (*pchStr1);
	}
	pchStr1--;
	a = (*pchStr1);

	int n = 0;
	int j = 0;
	while (a != '\n')
	{
		s[j][0] = '\0';

		int i = 0;
		pchStr1++;
		a = (*pchStr1);
		while (a != ' ')
		{
			if (a == '\n') break;
			s[j][i] = a;
			i++;
			pchStr1++;
			a = (*pchStr1);
		}
		s[j][i] = '\0';
		
		j++;
	}
	return TRUE;
}


//按照位置读取字符
BOOL TReadALine::GetStrFromStr2(const char *str1, const int start, const int length, char *s)
{
	//char *s;
	char a;
	char *pchStr1 = (char *)str1;
	
	char b = ' ';
	pchStr1 = pchStr1 + start;
	a = (*(pchStr1));
	for (int i=0;i<length;i++)
	{
		s[i] = a;
		pchStr1++;
		a = (*(pchStr1));		
	}
	s[length] = '\0';

	return TRUE;
}

BOOL TReadALine::GetNumFromStrs(const char *str1,const char *str2,char start,char &num,float *ff)
{
	char *p,*str,s[31],key;
	int i,k,i2,kk;
	char *pchStr1=(char *)str1;
	str=&(*(pchStr1+start));
	if(str[strlen(str)-1]!='\n')
	{
		strcat(str,"\n");
	}
	if(strlen(str2)==0)
	{
		p=str;
		goto nxtt;
	}
	if((p=strstr(str,str2))!=NULL)
	{
nxtt:	 k=0;
		 kk=0;
		 p=p+strlen(str2);
		 i2=strlen(p);
		 for(i=0;i<i2;i++)
		 {
			 key=*(p+i);
			 if(((key>='0')&&(key<='9'))||(key=='.')||(key=='+')
				 ||(key=='-')||(key=='e')||(key=='E'))
			 {
				 s[k]=key;
				 k++;
				 if(k>=30)
				 {
					 sprintf(str,"Line %ld in %s",line,FileName);
					 ////MessageBox(NULL,"A string for a double is too long!",
					////	 str,MB_OK | MB_ICONEXCLAMATION);
					 return FALSE;
				 }
			 }
			 else
				 //       if((key==' ')||(key==',')||(key==TAB)||(key=='\n'))
			 {
				 if(k==0) continue;
				 else
				 {
					 s[k]=0;
					 ff[kk]=float(atof(s));
					 kk++;
					 k=0;
					 if(kk>=num) return TRUE;
				 }
			 }
			 /*      else
			 {
			 sprintf(str,"%d char in line %d in %s",i,line,FileName);
			 MessageBox(States->MessWin->HWindow,"is unrecognized for double!",
			 str,MB_OK | MB_ICONEXCLAMATION);
			 return FALSE;
		 } */
		 }
    }
	//  sprintf(str,"Line %ld in %s",line,FileName);
	//  MessageBox(States->ChildWin.CanvasWin->HWindow,"No enough float to read!",
	//	 str,MB_OK | MB_ICONEXCLAMATION);
	//  return FALSE;
	num=kk;
	return TRUE;
}

//找到含有指定串的行，同时文件指针也到该行
BOOL TReadALine::FindString(FILE *stream, const char* str)
{
	char* szStr;
	while (endfile != TRUE)
	{
		szStr = ReadAString(stream);
		if (strstr(szStr, str) != NULL)
		{
			return TRUE;
		}
	}
	return FALSE;
}

/*
//找到含有指定串的行，同时文件指针也到该行
BOOL TReadALine::FindString2(FILE *stream, const char* str, CString& strString)
{
	char* szStr;
	while (endfile != TRUE)
	{
		szStr = ReadAString(stream);
		if (strstr(szStr, str) != NULL)
		{
			strString = szStr;
			return TRUE;
		}
	}
	return FALSE;
}
*/
