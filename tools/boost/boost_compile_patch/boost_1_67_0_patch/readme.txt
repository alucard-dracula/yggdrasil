��װ Microsoft HPC Pack 2008 SDK  // 64λMPI (��������D:\Program Files\Microsoft HPC Pack 2008 SDK)
��װ Microsoft Compute Cluster Pack // 32λMPI ���ֱ��ѹ������ѹ�����£���������D:\Program Files (x86)\Microsoft Compute Cluster Pack��
��װ PYTHON 2.7 ���ҽ�yggdrasil/tools/python���ļ� ���Ƶ�python�Ķ�ӦĿ¼
��װ ������ ICU

��ѹboost
��������̨ ���� boost Ŀ¼
bootstrap.bat
bjam toolset=msvc -sICU_PATH="�����õ�ICUĿ¼���ҵ���D:/icu_vc100)" --build-type=complete  //32λ
bjam toolset=msvc address-model=64 -sICU_PATH="�����õ�ICUĿ¼���ҵ���D:/icu_vc100)" --build-type=complete  //64λ

�ȱ������ ����Ƿ������� û������֮��
�� boostĿ¼�µĵ�stage�� �����ļ����Ƶ���Ӧ��bin��libĿ¼

ע��32λ��64λ�ֿ�����

�ļ��ĸ��Ʒ�����win32 win64��Ŀ¼��

// �÷����� boost_1_67_0 �� ʧЧ�� ��ʱû�ҵ�����취, ������python��·���� mpi_python���޷���ȷ����
���� Tried to build the target twice, with property sets having
1 ����ԭʼ�ļ�����һ�� �����޷����� MPI
2 ���ǲ��� ����
3 ��ѹmsvc.zip ԭʼ�ļ� �ٽ��б���