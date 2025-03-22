linux using dtl connect mysql:
1, install mysql (recommend x86_64)
2, install mysql-connector-odbc (x64 and x86)
3, install unixODBC (x64 and x86)
4, config mysql and test it

warning:
1, if dnf can't install mysql-connector-odbc.i686 manual install it
2, don't using mysql-connector-odbc 8.0.37 connect mysql 8.0.40 has bugs, see mysql-connector-odbc8.0.37/driver/connect.cc 
    foo "DBD::connect", "mysql = mysql_init(nullptr)"
