复制目录中文件替换boost中文件
修改boost_1_53_0\tools\build\v2\tools\mpi.jam 中 “Microsoft HPC Pack 2008 SDK”路径（可能是 Microsoft Compute Cluster Pack (32位)）

discard:
#复制 user-config.jam 到 boost_1_53_0\tools\build\v2 
#复制 mpi.jam 到 boost_1_53_0\tools\build\v2\tools
#复制 mpi/Jamfile.v2 到 boost_1_53_0\libs\mpi\build
