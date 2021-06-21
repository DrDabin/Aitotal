//---------------------------------------------------------------------------

#ifndef ParseHeadrArchivH
#define ParseHeadrArchivH
//---------------------------------------------------------------------------
   bool Parse_Head_Guid(UnicodeString guidParse,unsigned int &GuidArchiv);
   bool Parse_Head_Guid_New(UnicodeString guidParse,unsigned int &GuidArchiv);
   bool Read_head_file(UnicodeString FileName, UnicodeString &head);
   bool Read_head_file_New(UnicodeString FileName, UnicodeString &head);

#endif
