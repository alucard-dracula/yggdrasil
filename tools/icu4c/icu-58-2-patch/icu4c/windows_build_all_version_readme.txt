1, build with vs high version (e.g vs2022)
	1.1, run source/format_vcxproj.sh
	1.2, run icu4c_build_vc143_no_versioned.bat
	1.3, run source/vcxproj_org_to_versioned.sh
	1.4, run icu4c_build_vc143_versioned.bat
	1.5, run source/vcxproj_ver_md_to_ver_mt.sh
	1.6, run icu4c_build_vc143_versioned_mt.bat

2, revoke to icu org version
	2.1, run source/vcxproj_ver_mt_to_ver_md.sh
	2.2, run source/vcxproj_versioned_to_org.sh

3, build with vs low version (e.g vs2010) 
	3.1, run source/downgrade_vcxproj.sh
	3.2, run icu4c_build_vc100_no_versioned.bat
	3.3, run source/vcxproj_org_to_versioned.sh
	3.4, run icu4c_build_vc100_versioned.bat
	3.5, run source/vcxproj_ver_md_to_ver_mt.sh
	3.6, run icu4c_build_vc100_versioned_mt.bat
