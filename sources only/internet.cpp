#include"internet.h"

void CInternet::GetData(ofstream& output)
{
	wxHTTP	 http;
	http.SetHeader(_T("Accept"),_T("text/*"));
	http.SetHeader(_T("User-Agent"),_T("Mozilla"));
	http.SetTimeout(10);

	wxString	server;
	wxString	path;
	wxString	msg;

	server="www.findfreehosts.com";
	// This path will work fine 
	// (because it can't find a direct search result?)
	path="/lsd.dat";
	// This path will cause 0 bytes to be returned
	// path="/find?s=all&q=the+medusa+touch";

	if (http.Connect(server,80))
	{
		wxInputStream	*stream;
		stream = http.GetInputStream(path);
		if (stream == NULL)
		{
			throw 1;
		}
		else
		{
			char buffer[1024];
			int	 bytesRead;
			string s;
			while (!stream->Eof())
			{
				stream->Read(buffer,sizeof(buffer));
				bytesRead=stream->LastRead();
				if (bytesRead<=0)
				{
					break;
				}
				output.write(buffer,bytesRead);
				wxString	msg;
				msg.Printf("read %d bytes",bytesRead);
				wxMessageBox(msg);
			}
		}
	}
	else
	{
		throw 1;
	}
}