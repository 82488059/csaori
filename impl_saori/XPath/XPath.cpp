
#ifdef _MSC_VER
#pragma warning( disable : 4786 )
#endif

#include "csaori.h"
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <time.h>

#include "tinyxml.h"
#include "xpath_processor.h"

//////////WINDOWS DEFINE///////////////////////////
//include�̂��Ƃɂ����ĂˁI
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
///////////////////////////////////////////////////

class XMLDocInfo
{
private:
	XMLDocInfo() { }
public:
	TiXmlDocument *xml;
	unsigned int id;
	string_t filepath;
	unsigned int cp;

	XMLDocInfo(TiXmlDocument *x,unsigned int i,string_t f,unsigned int c) :
		xml(x) , id(i) , filepath(f) , cp(c) { }
	~XMLDocInfo() {
		delete xml;
	}
};

typedef std::vector<XMLDocInfo *> XMLPtrVector;

static XMLPtrVector g_xml;

/*---------------------------------------------------------
	������
---------------------------------------------------------*/
bool CSAORI::load()
{
	srand(time(NULL));
	return true;
}

/*---------------------------------------------------------
	���
---------------------------------------------------------*/
bool CSAORI::unload()
{
	for ( XMLPtrVector::iterator it = g_xml.begin() ; it != g_xml.end() ; ++it ) {
		delete *it;
	}
	g_xml.clear();
	return true;
}

/*---------------------------------------------------------
	���s
---------------------------------------------------------*/
void CSAORI::exec(const CSAORIInput& in,CSAORIOutput& out)
{
	//�W���F�o�b�h���N�G�X�g
	//����ɂ����邱�Ƃ��킩�������_��200/204�ɏ���������
	out.result_code = SAORIRESULT_BAD_REQUEST;
	if ( ! in.args.size() ) {
		return;
	}

	if ( wcsicmp(in.args[0].c_str(),L"open") == 0 ) {
		//���́Fopen,�t�@�C����
		//�o�́F�t�@�C��ID
		if ( in.args.size() < 2 ) {
			return;
		}

		//�����̃t�@�C�����J���Ă���ė��p
		for ( XMLPtrVector::iterator it = g_xml.begin() ; it != g_xml.end() ; ++it ) {
			if ( (**it).filepath == in.args[1] ) {
				out.result = SAORI_FUNC::intToString((**it).id);

				out.result_code = SAORIRESULT_OK;
				return;
			}
		}

		//�ӂ����邨�[�ۂ�I
		std::string fname = checkAndModifyPath(SAORI_FUNC::UnicodeToMultiByte(in.args[1]));

		TiXmlDocument *doc = new TiXmlDocument;
		if ( ! doc->LoadFile(fname.c_str()) ) {
			delete doc;
			return;
		}

		//�ŏ��̎q�����邩�m�F
		TiXmlNode *pNodeDec = doc->FirstChild();
		if ( ! pNodeDec ) {
			delete doc;
			return;
		}

		//��`
		unsigned int cp;
		TiXmlDeclaration *pDec = pNodeDec->ToDeclaration();
		if ( pDec && strlen(pDec->Encoding()) ) {
			cp = SAORI_FUNC::StringtoCodePage(pDec->Encoding());
		}
		else {
			cp = CP_UTF8;
		}

		//�Ǘ��\���̂ɂ������200���^�[��
		unsigned int id = rand();

		XMLDocInfo *pInfo = new XMLDocInfo(doc,id,in.args[1],cp);
		g_xml.push_back(pInfo);

		out.result = SAORI_FUNC::intToString(id);
		out.result_code = SAORIRESULT_OK;
	}
	else if ( wcsicmp(in.args[0].c_str(),L"close") == 0 ) {
		//���́Fclose,�t�@�C��ID

		if ( in.args.size() < 2 ) {
			return;
		}

		//�P���ɒT���Č���������{�b�V���[�g
		//�����Ђǂ����Ƃ��N������ID�����Ԃ�ꍇ�ɔ����đS���Ȃ߂�
		unsigned int id = wcstoul(in.args[1].c_str(),NULL,10);
		bool found = false;

		for ( XMLPtrVector::iterator it = g_xml.begin() ; it != g_xml.end() ; ++it ) {
			if ( (**it).id == id ) {
				delete *it;
				it = g_xml.erase(it);
				found = true;

				if ( it == g_xml.end() ) { break; }
			}
		}

		if ( found ) {
			out.result_code = SAORIRESULT_NO_CONTENT;
		}
	}
	else if ( wcsicmp(in.args[0].c_str(),L"get") == 0 ) {
		//���́Fget,�t�@�C��ID,XPath

		if ( in.args.size() < 3 ) {
			return;
		}

		//�P���ɒT���Č����邾��
		unsigned int id = wcstoul(in.args[1].c_str(),NULL,10);
		bool found = false;
		XMLPtrVector::iterator it = g_xml.begin();

		for ( ; it != g_xml.end() ; ++it ) {
			if ( (**it).id == id ) {
				break;
			}
		}

		if ( it == g_xml.end() ) {
			return;
		}

		std::string xpath = SAORI_FUNC::UnicodeToMultiByte(in.args[2],(**it).cp);
		try {
			TinyXPath::xpath_processor xpath((**it).xml->FirstChild(),xpath.c_str());

			TinyXPath::expression_result r = xpath.er_compute_xpath();

			if ( r.e_type == TinyXPath::e_bool ) {
				out.result = r.o_get_bool() ? L"1" : L"0";
				out.values.push_back(out.result);
			}
			else if ( r.e_type == TinyXPath::e_string ) {
				TiXmlString str = r.S_get_string();
				out.result = SAORI_FUNC::MultiByteToUnicode(str.c_str(),(**it).cp);
				out.values.push_back(out.result);
			}
			else if ( r.e_type == TinyXPath::e_int ) {
				std::wostringstream os;
				os << r.i_get_int();
				out.result = os.str();
				out.values.push_back(out.result);
			}
			else if ( r.e_type == TinyXPath::e_double ) {
				std::wostringstream os;
				os << r.d_get_double();
				out.result = os.str();
				out.values.push_back(out.result);
			}
			else if ( r.e_type == TinyXPath::e_node_set ) {
				TinyXPath::node_set *np = r.nsp_get_node_set();
				if ( np ) {
					size_t n = np->u_get_nb_node_in_set();
					string_t wstr;
					TiXmlString str;

					for ( size_t i = 0 ; i < n ; ++i ) {
						if ( np->o_is_attrib(i) ) {
							const TiXmlAttribute *ap = np->XAp_get_attribute_in_set(i);
							if ( ap ) {
								wstr = SAORI_FUNC::MultiByteToUnicode(ap->Value(),(**it).cp);
								out.result += wstr;
								out.result += L"\1";
								out.values.push_back(wstr);
							}
						}
						else {
							const TiXmlNode *xp = np->XNp_get_node_in_set(i);
							if ( xp ) {
								str.clear();
								const TiXmlText *tp = xp->ToText();
								if ( tp ) {
									str = tp->Value();
								}
								else {
									const TiXmlNode *cpl = xp->LastChild();
									const TiXmlNode *cp = xp->FirstChild();
									if ( cp && cpl && cp == cpl ) { //�q���ЂƂ����{�e�L�X�g�m�[�h
										tp = cp->ToText();
										if ( tp ) {
											str = tp->Value();
										}
									}
								}
								if ( str.length() ) {
									wstr = SAORI_FUNC::MultiByteToUnicode(str.c_str(),(**it).cp);
									out.result += wstr;
									out.result += L"\1";
									out.values.push_back(wstr);
								}
							}
						}
					}
				}
				if ( out.result.size() ) {
					out.result.erase(out.result.end()-1,out.result.end());
				}
			}
			out.result_code = SAORIRESULT_OK;
		}
		catch(...) {
			out.result_code = SAORIRESULT_INTERNAL_SERVER_ERROR;
		}
	}
}

