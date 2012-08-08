#include "stdafx.h"
#include "SimpleSendMail.h"
#include "SMTP.h"

CSimpleSendMail::CSimpleSendMail()
{
	WORD	vwVersion;
	WSADATA	vtWsaData;

	vwVersion = MAKEWORD(1, 1);

	WSAStartup(vwVersion, &vtWsaData);
}

CSimpleSendMail::~CSimpleSendMail()
{
}

bool CSimpleSendMail::SendMail(LPCTSTR		apszMailServerName,
						   CStringArray	&aarrToEmailAddresses,
						   CStringArray	&aarrToDisplayNames,
						   LPCTSTR		apszFromEmailAddress,
						   LPCTSTR		apszFromDisplayName,
						   LPCTSTR		apszSubject,
						   LPCTSTR		apszBodyText,
						   bool			abIsHTML,
						   CStringArray	&aarrAttachments,
						   int			aiSMTPPortNumber,
						   LPCTSTR		aszCustomHELOParameter)
{
	bool			vbRet = false;
	bool			vbContinue;
	CSMTPConnection	vtConnection;
	CSMTPMessage	vtMessage;
	CSMTPAddress	vtTo;
	CSMTPAddress	vtFrom;
	CSMTPAttachment	*vatAttachment = NULL;
	int				i;

	if (vtConnection.Connect(apszMailServerName, aiSMTPPortNumber, aszCustomHELOParameter))
	{
		vbContinue = true;

		for (i = 0; (i < aarrToEmailAddresses.GetSize()) && vbContinue; ++i)
		{
			vtTo.m_sEmailAddress = aarrToEmailAddresses[i];
			if (i < aarrToDisplayNames.GetSize())
			{
				vtTo.m_sFriendlyName = aarrToDisplayNames[i];
			}
			else
			{
				vtTo.m_sFriendlyName = vtTo.m_sEmailAddress;
			}
			
			vbContinue = (vtMessage.AddRecipient(vtTo) >= 0);
		}

		if (vbContinue)
		{
			if (aarrAttachments.GetSize() > 0)
			{
				vatAttachment = new CSMTPAttachment[aarrAttachments.GetSize()];
			}

			for (i = 0; (i < aarrAttachments.GetSize()) && vbContinue; ++i)
			{
				vbContinue = vatAttachment[i].Attach(aarrAttachments[i]) &&
							 (vtMessage.AddAttachment(&vatAttachment[i]) >= 0);
			}

			if (vbContinue)
			{
				vtMessage.m_From.m_sEmailAddress = apszFromEmailAddress;
				vtMessage.m_From.m_sFriendlyName = apszFromDisplayName;
				vtMessage.m_sSubject = apszSubject;
				vtMessage.AddBody(apszBodyText, abIsHTML);
				
				vbRet = (vtConnection.SendMessage(vtMessage) != FALSE);
			}

			if (vatAttachment != NULL)
			{
				delete [] vatAttachment;
			}
		}
	}
	return vbRet;
}
