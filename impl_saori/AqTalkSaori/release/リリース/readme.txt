----------------------------------------------------------------------
���uAquesTalkSaori�v�FAquesTalk�ɂ�鉹������SAORI
��Written by ukiya
�@mailto:ukiya@s.mail-box.ne.jp
�@http://ukiya.sakura.ne.jp/
----------------------------------------------------------------------

������͉���������̂�

�@������ЃA�N�G�X�g�̊J�������AAquesTalk�Ƃ������������G���W���𗘗p���āA
�@�����������s��SAORI�ł��B

�������

�@�EAqTalkSaori.dll�AAquesTalk.dll�A�𓯂��f�B���N�g���ɂ���Ă��������B
�@�E�f�t�H���g�ł͏����̐��ł����AAquesTalk.dll �������ւ��邱�Ƃɂ����
�@�@�ʂ̐��ɂȂ�܂��B�����́u���̐��v�f�B���N�g�����Q�Ƃ��Ă��������B

�@�E�i�Q�l�j�J�����FWindows Vista Ultimate + Core2 2.13GHz + 2GB Memory
�@�@�@�@�@  C++ on VisualStudio.NET2005


���g�p���@

������̋@�\���A�����L����̓��͂��K�v�ƂȂ�܂��B
�����L����Ƃ́A��ɂЂ炪�Ȃō\�����ꂽ�A�����̕�����ł��B
�ڂ����͓����́u�����L����d�l.pdf�v���Q�Ƃ��Ă��������B

�i�P�jPLAYSYNC

�y�@�@�@�\�z
�@�@���͂��������L��������̏�ōĐ�����B�Đ������܂Ŏ��s���X�g�b�v���܂��B
�y���@�@���z
�@�@Argument0: PLAYSYNC
�@�@Argument1: �����L����
�@�@Argument2: �X�s�[�h�i�ȗ��\�j�i50�`300�A�ȗ���100�j
�y���ӎ����z
�@�@�E�Đ��������܂Ő��䂪�߂�܂���B
�y��z
�@	_result=FUNCTIONEX("AqTalkSaori.dll","PLAYSYNC","�ق��ق��ւ��ւ�");

�i�Q�jCREATEWAV

�y�@�@�@�\�z
�@�@���͂��������L�����WAV�t�@�C���ɂ���B
�y���@�@���z
�@�@Argument0: CREATEWAV
�@�@Argument1: �����L����
�@�@Argument2: �X�s�[�h�i�ȗ��ł��܂���j�i50�`300�A���ʂ�100�j
�@�@Argument3: �t�@�C�����i�g���q�܂ށj
�y���ӎ����z
�@�@�E�t�@�C����DLL�̂���f�B���N�g���ɍ쐬����܂��B
�@�@�E�쐬�����WAV�t�@�C���͈ȉ��̃t�H�[�}�b�g�ł��B
�@�@�@8kHz 16bit ���m���� ���j�APCM WAV�`��
�@�@�E���̃t�@�C�������ɊJ����Ă���ꍇ�͍쐬�ł����ɃG���[�ƂȂ�܂��B
�y��z
//	_result=FUNCTIONEX("AqTalkSaori.dll","CREATEWAV","�ق��ւ��ق��ւ�",100,"hoge.wav");

�i�R�jPLAYASYNC

�y�@�@�@�\�z
�@�@���͂��������L��������̏�ōĐ�����B�Đ�������҂����ɐ����Ԃ��܂��B
�y���@�@���z
�@�@Argument0: PLAYASYNC
�@�@Argument1: �����L����
�@�@Argument2: �X�s�[�h�i�ȗ��\�j�i50�`300�A�ȗ���100�j
�y���ӎ����z
�@�@�E�Đ����Ƃ߂����ꍇ��STOP�R�}���h�𗘗p���Ă��������B
�@�@�E�Đ�����PLAYSYNC,PLAYASYNC,CREATEWAV�R�}���h�����s����ƍĐ����~�܂�܂��B
�y��z
�@	_result=FUNCTIONEX("AqTalkSaori.dll","PLAYASYNC","�ق��ق��ւ��ւ�");

�i�S�jSTOP

�y�@�@�@�\�z
�@�@PLAYASYNC�ōĐ����̉������~����B
�y���@�@���z
�@�@Argument0: STOP
�y���ӎ����z
�y��z
�@	_result=FUNCTIONEX("AqTalkSaori.dll","STOP);

�i�T�jCHANGEVOICE

�y�@�@�@�\�z
�@�@���s���ɗ��p����DLL�������ւ���B
�y���@�@���z
�@�@Argument0: CHANGEVOICE
�@�@Argument1: DLL�t�@�C����
�y���ӎ����z
�@�@�E�����ւ��Ώۂ̃t�@�C������ AqTalkSaori.dll �Ɠ����f�B���N�g���ɂ����Ă��������B
�y��z
�@	_result=FUNCTIONEX("AqTalkSaori.dll","CHANGEVOICE","m1.dll");


���G���[�R�[�h

SAORI���Ԃ��\���̂���G���[��񋓂��܂��B

ERROR001�F����������܂���
ERROR301�FDLL�t�@�C�����w�肳��Ă��܂���
ERROR302�FDLL�t�@�C���̃��[�h�Ɏ��s���܂����iDLL�t�@�C�����j
ERROR003�F�s���Ȗ��߂ł�
ERROR999�FAquesDLL���[�h�Ɏ��s���Ă��܂�
ERROR002�F�Œ�Q�ȏ�̈������w�肵�Ă�������
ERROR100�FSynthe�̃G���[(�G���[�R�[�h)
ERROR101�Fmalloc�̃G���[
ERROR201�F�t�@�C�������w�肳��Ă��܂���
ERROR202�F�t�@�C�����J���̂Ɏ��s���܂���


�uERROR100�FSynthe�̃G���[(�G���[�R�[�h)�v���Ԃ����ꍇ�́A
AquesTalk�����̃G���[�ł��B�ȉ��̃G���[�R�[�h���Q�l�ɂ��Ă��������B
	100	���̑��̃G���[
	101	�������s��
	102	�����L����ɖ���`�̓ǂ݋L�����w�肳�ꂽ
	103	�C���f�[�^�̎��Ԓ����}�C�i�X�ɂȂ��Ă���
	104	�����G���[�i����`�̋�؂�R�[�h���o�j
	105	�����L����ɖ���`�̓ǂ݋L�����w�肳�ꂽ
	106	�����L����̃^�O�̎w�肪�������Ȃ�
	107	�^�O�̒������������z���Ă���
	108	�^�O���̒l�̎w�肪�������Ȃ�
	109	WAVE�Đ����ł��Ȃ��i�T�E���h�h���C�o�֘A�̖��j
	110	�{�v���O�����ł͖��g�p
	111	�������ׂ��f�[�^���Ȃ�
	200	�����L���񂪒�������
	201	�P�̃t���[�Y���̓ǂ݋L������������
	202	�����L���񂪒����i�����o�b�t�@�I�[�o�[�P�j
	203	�q�[�v�������s��
	204	�����L���񂪒����i�����o�b�t�@�I�[�o�[�P�j

���z�z������

�ȉ���DLL�t�@�C���A�h�L�������g�͊�����ЃA�N�G�X�g�̒��앨�ł��B
�����̃t�@�C���̔z�z�����́A������AqLicence.txt�ɏ]���ĉ������B

�EAqLicence.txt
�EAquesTalk.dll ����сu���̐��v�f�B���N�g���ɂ���DLL
�E�����L����d�l.pdf

����ȊO�̃t�@�C���́A���q���̒��앨�ł��B
�����ɂ��Ă͂��D���Ȃ悤�ɂǂ����B

�����������������������������@���Ӂ@������������������������������������������
���̃Z�b�g���S�[�X�g�ɗ��p���Ĕz�z����ۂ́ADLL�Ɠ����ꏊ��AqLicence.txt���܂߁A
�ȉ��̕����S�[�X�g��ReadMe���ǂ����ɋL�ڂ��Ă��������B
����Ń��C�Z���X�𖞂����Ǝv���܂��B

�E���̃S�[�X�g�́A������ЃA�N�G�X�g�̋K�������������C�u����AquesTalk���g�p���Ă��܂��B
�EAquesTalk �K�������������C�u���� AquesTalk.dll AquesTalkDa.dll �́A������ЃA�N�G�X�g �̒��앨�ł��B
������������������������������������������������������������������������������


���X�V����

2008/02/02	ver1.0.0.0	�V�K�쐬
2008/02/02	ver1.0.1.0	���X�Ŏ��������[�h���s���ƃ��C�u���������[�h�ł��Ȃ����ɑΉ�
2008/02/02	ver1.0.2.0	�t�@�C���o�͂̃p�X�𖾎�
2008/03/01	ver1.1.0.0	PLAYASYNC�ASTOP�ACHANGEVOICE��ǉ��BCREATEWAV�̈��������ύX�B
2008/03/01	ver1.1.1.0	CREATEWAV������ɓ����Ȃ������C���A�G���[���ɗ���������C��
2010/04/02	ver2.0.0.0	CSAORI�x�[�X�ɍX�V�i�@�\�ǉ������j

��Thanks to

�@�E���̃v���O�����́A���������� AquesTalk �K�������������C�u���� ���g�p���Ă��܂��B
�@�E�{�v���O�����ł� AquesTalk.dll �𗘗p���Ă��܂��B
�@�EAquesTalk �K�������������C�u���� AquesTalk.dll �́A������ЃA�N�G�X�g �̒��앨�ł��B
�@�EAquesTalk �K�������������C�u�����̃��C�Z���X�� AqLicence.txt ���Q�Ƃ��������B
�@�Ehttp://www.a-quest.com/aquestalk/index_win.html

�@�f�������\�t�g�E�F�A���J���E���J����Ă���F�l�ɐ[�����Ӓv���܂��B

