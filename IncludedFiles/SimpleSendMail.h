#ifndef SIMPLESENDMAIL_H
#define	SIMPLESENDMAIL_H

class CSimpleSendMail
{
public:
	CSimpleSendMail();
	virtual ~CSimpleSendMail();

	bool	SendMail(LPCTSTR		apszMailServerName,
					 CStringArray	&aarrToEmailAddresses,
					 CStringArray	&aarrToDisplayNames,
					 LPCTSTR		apszFromEmailAddress,
					 LPCTSTR		apszFromDisplayName,
					 LPCTSTR		apszSubject,
					 LPCTSTR		apszBodyText,
					 bool			abIsHTML,
					 CStringArray	&aarrAttachments,
					 int			aiSMTPPortNumber = 25,
					 LPCTSTR		aszCustomHELOParameter = NULL);

};

#endif