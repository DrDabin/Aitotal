//---------------------------------------------------------------------------

#ifndef XeshSummaH
#define XeshSummaH

class XeshSumma{

   private:
		   bool XeshSumma::CalcHash(UnicodeString FileName, DWORD dwProvType, ALG_ID Algid, UnicodeString &OutHash, UnicodeString &OutError);
		   //~XeshSumma();


   public:
		  bool XeshSumma::SHA256(UnicodeString FileName, UnicodeString &sha256, UnicodeString &error);




};


//---------------------------------------------------------------------------
#endif
