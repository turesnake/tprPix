//
//========================= __sqlite3.h ==========================
//                         -- OpenGL --
//                                        创建 -- 2019.04.20
//                                        修改 -- 
//----------------------------------------------------------
//   sqlite3 API
//----------------------------

//------ FAKE structs / types -------//
struct sqlite3{ //- fake
    int padding;
};
struct sqlite3_stmt{ //- fake
    int padding;
};
using sqlite3_int64 = long long; //- fake
using sqlite3_uint64 = unsigned long long; //- fake
using sqlite3_value = int; //- fake



//------ COPYED ret vals -------//
#define SQLITE_OK           0   /* Successful result */
/* beginning-of-error-codes */
#define SQLITE_ERROR        1   /* Generic error */
#define SQLITE_INTERNAL     2   /* Internal logic error in SQLite */
#define SQLITE_PERM         3   /* Access permission denied */
#define SQLITE_ABORT        4   /* Callback routine requested an abort */
#define SQLITE_BUSY         5   /* The database file is locked */
#define SQLITE_LOCKED       6   /* A table in the database is locked */
#define SQLITE_NOMEM        7   /* A malloc() failed */
#define SQLITE_READONLY     8   /* Attempt to write a readonly database */
#define SQLITE_INTERRUPT    9   /* Operation terminated by sqlite3_interrupt()*/
#define SQLITE_IOERR       10   /* Some kind of disk I/O error occurred */
#define SQLITE_CORRUPT     11   /* The database disk image is malformed */
#define SQLITE_NOTFOUND    12   /* Unknown opcode in sqlite3_file_control() */
#define SQLITE_FULL        13   /* Insertion failed because database is full */
#define SQLITE_CANTOPEN    14   /* Unable to open the database file */
#define SQLITE_PROTOCOL    15   /* Database lock protocol error */
#define SQLITE_EMPTY       16   /* Internal use only */
#define SQLITE_SCHEMA      17   /* The database schema changed */
#define SQLITE_TOOBIG      18   /* String or BLOB exceeds size limit */
#define SQLITE_CONSTRAINT  19   /* Abort due to constraint violation */
#define SQLITE_MISMATCH    20   /* Data type mismatch */
#define SQLITE_MISUSE      21   /* Library used incorrectly */
#define SQLITE_NOLFS       22   /* Uses OS features not supported on host */
#define SQLITE_AUTH        23   /* Authorization denied */
#define SQLITE_FORMAT      24   /* Not used */
#define SQLITE_RANGE       25   /* 2nd parameter to sqlite3_bind out of range */
#define SQLITE_NOTADB      26   /* File opened that is not a database file */
#define SQLITE_NOTICE      27   /* Notifications from sqlite3_log() */
#define SQLITE_WARNING     28   /* Warnings from sqlite3_log() */
#define SQLITE_ROW         100  /* sqlite3_step() has another row ready */
#define SQLITE_DONE        101  /* sqlite3_step() has finished executing */
/* end-of-error-codes */


/* ===========================================================
 * 
 * -----------------------------------------------------------
 *                       
 */

/* -----------------------------------------------------------
 *                       
 */


/* ===========================================================
 *                   error msg
 * -----------------------------------------------------------
 * -- 将 sqlite 各函数返回值 rc，作为参数 传入此函数
 *    获得其 出错信息                       
 */
const char *sqlite3_errstr(int);


/* ===========================================================
 *                        config
 * -----------------------------------------------------------
 * -- 用来改写 sqlite 参数。
 * -- 此函数不是线程安全的，应确保在此函数调用时，没有其他线程在 使用 sqlite 接口。
 * 
 * -- 本函数只能在 sqlite3_initialize()之前， 或 sqlite3_shutdown() 之后 调用
 *    若在这两函数之间调用，将返回 SQLITE_MISUSE
 * 
 * -- 参数1 为 Configuration Options，表示想要修改 哪个 特性
 *    后续参数 取决于 参数1
 * 
 * -- return：
 *    调用成功，返回 SQLITE_OK。
 *    若 参数 无法识别，或设置失败，返回一个 非0 的 error code
 * 
 * 
 *                       
 */
int sqlite3_config(int, ...);



/* ===========================================================
 *                       open
 * -----------------------------------------------------------
 * -- 就算调用出错，也会向 参数 ppDb 返回 db-handle。
 *    仅在 sqlite 无法为新 db对象 分配内存时，才会向 ppDb 写入 NULL
 * 
 * -- sqlite3_open() / sqlite3_open_v2() 的默认编码格式为 -- UTF-8
 *    sqlite3_open16() 的格式为 -- UTF-16
 *    ----
 *    这一区别，也影响在它们 读取的参数 filename 的格式上
 * 
 * -- 本函数不管是否发生错误，都应调用 sqlite3_close() 将中途绑定的资源 释放
 * 
 * -- 若参数 filename 为 “:memory:”，或 NULL。将生成一个 位于 内存的临时 数据库
 *    这个数据库会在 连接终止后 消失。
 * 
 * -- 为避免歧义，如果 filename 以 “:” 开头，应添加前缀 “./” 来规避（其他路径前缀也行）
 * 
 * -- 参数 filename 也可以时 URI 格式（网址格式）
 *    更多信息请查看 API
 * 
 * -- return
 *    若正确，函数返回  SQLITE_OK, 即0.
 *    若出错，返回各种 error code... 可使用 sqlite3_errmsg() / sqlite3_errmsg16()
 *    来 打印错误信息
 */
int sqlite3_open(
    const char *filename,   /* Database filename (UTF-8) */
    sqlite3 **ppDb          /* OUT: SQLite db handle */
);

int sqlite3_open16(
    const void *filename,   /* Database filename (UTF-16) */
    sqlite3 **ppDb          /* OUT: SQLite db handle */
);

int sqlite3_open_v2(
    const char *filename,   /* Database filename (UTF-8) */
    sqlite3 **ppDb,         /* OUT: SQLite db handle */
    int flags,              /* Flags */
    const char *zVfs        /* Name of VFS module to use */
);




/* ===========================================================
 *                         exec
 *                    一键式 SQL 命令
 * -----------------------------------------------------------
 * -- 本接口是  
 *      sqlite3_prepare_v2()
 *      sqlite3_step()
 *      sqlite3_finalize()
 *    的一个便捷封装。
 * 
 * -- 通过第二个参数 传入一组 由分号分割的，UTF-8 格式的 SQL 语句
 *    如果 参数2 为 NULL，或 空字符串，或 空格／分号。 那么数据库状态 不会被改变。
 *    数据也不会被改写
 * 
 * -- 三号参数为回调函数，如果不为 NULL，
 *    sqlite 将为 参数sql 内执行的每个 SELECT 语句中处理每个记录时，调用此callback      
 */
int sqlite3_exec(
    sqlite3* db,                               /* An open database */
    const char *sql,                           /* SQL to be evaluated */
    int (*callback)(void*,int,char**,char**),  /* Callback function */
    void *,                                    /* 1st argument to callback */
    char **errmsg                              /* Error msg written here */
);

//-- 参数3 的细节 --
typedef int (*sqlite3_callback)(
    void*,    /* Data provided in the 4th argument of sqlite3_exec() */
    int,      /* The number of columns in row */
    char**,   /* An array of strings representing fields in the row */
    char**    /* An array of strings representing column names */
);




/* -----------------------------------------------------------
 *                    更加精细的操作
 * -----------------------------------------------------------
 * sqlite3_prepare_v2() -- 创建这个对象
 * sqlite3_bind_*()     -- 给宿主参数（host parameters）绑定值
 * sqlite3_step()       -- 一次或多次来执行这个sql
 * sqlite3_reset()      -- 重置这个语句，然后回到第2步，这个过程做0次或多次
 * sqlite3_finalize()   -- 销毁这个对象   
 */



/* ===========================================================
 *                        prepare
 * -----------------------------------------------------------
 * -- Compiling An SQL Statement
 *    在 自行 sql语句之前，需要先将其编译。
 * 
 * -- 推荐使用 sqlite3_prepare_v2()
 *    sqlite3_prepare() 接口太老了，不推荐。
 *    sqlite3_prepare_v3() 需要额外参数：prepFlags，仅用于部分场合。
 * 
 * -- 推荐使用 UTF-8 格式的。
 * 
 * -- 若参数 nByte 为负数，sqlite将自动读取 zSql 数据，直到遇到第一个 尾后0
 *    若参数 nByte 为正数，则执行读取字节数。
 *    若参数 nByte 为0， 则不读取也不编译出 prepared statement。
 *    ---
 *    若调用者知道 zSql 以 尾后0 结尾。应该向 nByte 传入 要编译字节数（包含尾后0）
 *    这样可以避免， sqlite 复制此字符串，提高效率
 * 
 * -- pzTail：zSql在遇见终止符或者是达到设定的nByte之后结束，假如zSql还有剩余的内容，
 *    那么这些剩余的内容被存放到 pZTail中，不包括终止符。
 *    如果 pszTail 不为 NULL, 则 *pszTail 指向 sql 中第一个被传入的 SQL 语句的结尾. 
 *    该函数只编译 sql 的第一个语句, 所以 *pszTail 指向的内容则是未被编译的.
 * 
 * -- ppStmt：能够使用sqlite3_step()执行的编译好的准备语句的指针，
 *    如果错误发生，它被置为NULL，假如输入的文本不包括 sql 语句。调用过程必须负责
 *    在编译好的sql语句完成使用后使用 sqlite3_finalize() 删除它。
 * 
 * -- return：
 *     如果执行成功，则返回SQLITE_OK，否则返回一个错误码。
 * 
 */
int sqlite3_prepare(
    sqlite3 *db,            /* Database handle */
    const char *zSql,       /* SQL statement, UTF-8 encoded */
    int nByte,              /* Maximum length of zSql in bytes. */
    sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
    const char **pzTail     /* OUT: Pointer to unused portion of zSql */
);
int sqlite3_prepare_v2(   // <-- 推荐版本 --
    sqlite3 *db,            /* Database handle */
    const char *zSql,       /* SQL statement, UTF-8 encoded */
    int nByte,              /* Maximum length of zSql in bytes. */
    sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
    const char **pzTail     /* OUT: Pointer to unused portion of zSql */
);

int sqlite3_prepare_v3(
    sqlite3 *db,            /* Database handle */
    const char *zSql,       /* SQL statement, UTF-8 encoded */
    int nByte,              /* Maximum length of zSql in bytes. */
    unsigned int prepFlags, /* Zero or more SQLITE_PREPARE_ flags */
    sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
    const char **pzTail     /* OUT: Pointer to unused portion of zSql */
);
int sqlite3_prepare16(
    sqlite3 *db,            /* Database handle */
    const void *zSql,       /* SQL statement, UTF-16 encoded */
    int nByte,              /* Maximum length of zSql in bytes. */
    sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
    const void **pzTail     /* OUT: Pointer to unused portion of zSql */
);
int sqlite3_prepare16_v2(
    sqlite3 *db,            /* Database handle */
    const void *zSql,       /* SQL statement, UTF-16 encoded */
    int nByte,              /* Maximum length of zSql in bytes. */
    sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
    const void **pzTail     /* OUT: Pointer to unused portion of zSql */
);
int sqlite3_prepare16_v3(
    sqlite3 *db,            /* Database handle */
    const void *zSql,       /* SQL statement, UTF-16 encoded */
    int nByte,              /* Maximum length of zSql in bytes. */
    unsigned int prepFlags, /* Zero or more SQLITE_PREPARE_ flags */
    sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
    const void **pzTail     /* OUT: Pointer to unused portion of zSql */
);


/* ===========================================================
 *                        bind
 * -----------------------------------------------------------
 * -- bind*函数 必须要在 sqlite3_step() 前, sqlite3_prepare() 或 sqlite3_reset() 之后调用.
 * 
 * -- 参数2（_idx），要绑定第几个数据（sql句中第几个问号）。以 1 为首序号。
 *    如果 sql语句中使用了 命名的变量。可以通过 sqlite3_bind_parameter_index() 来计算出此序号。
 * 
 * -- 参数3（_val）就是要绑定的值。
 *    如果 sqlite3_bind_text()，sqlite3_bind_text16()，sqlite3_bind_blob()
 *    的 参数3 是 NULL。此时，参数4会被忽略。这几个函数运行起来，
 *    会和 sqlite3_bind_null() 效果一样。
 * 
 * -- 参数4（_valSize） 就是传入数据的 字节长度
 *    如果传入负数，sqlite将自行计算长度
 *    如果向 sqlite3_bind_blob() 的 参数4 传入负值，结果为定义。
 * 
 * -- 参数5，析构回调函数，
 *    就算本次函数调用失败，也会使用此 析构回调函数 来释放 stirng or blob
 *    若设置为 NULL，或者 参数4为 负值。此析构回调函数将不会被调用
 *    ---
 *    若设为 SQLITE_STATIC
 *    若设为 SQLITE_TRANSIENT， sqlite将自行复制一份数据（数据？）
 * 
 * -- 参数6（_encoding） 为 sqlite3_bind_text64() 特有，
 *    必须是如下值之一：
 *       SQLITE_UTF8
 *       SQLITE_UTF16
 *       SQLITE_UTF16BE
 *       SQLITE_UTF16LE 
 *    用来设置 参数3(_val) 的解析格式
 *    若传入值 不为上述之一，或者 传入值，于 参数3实际格式 不相符，则结果未定义。
 * 
 * -- 未绑定的参数，默认值为 NULL。
 * 
 * -- sqlite3_bind_zeroblob() 绑定一个 参数n字节的 BLOB，此 blob 全被0填充。
 *    zeroblob 使用一段 定长内存，常用于 先占一个空间，后期通过  incremental BLOB I/O  写入数据
 * 
 * -- sqlite3_bind_pointer() 中，存在特殊参数 _type。它必须为一个 static 字符串。
 * 
 * -- return 
 *    SQLITE_OK -- 调用成功
 *                       
 */
int sqlite3_bind_blob(sqlite3_stmt *_pStmt, int _idx, const void *_val, int _valSize, void(*_callback)(void*));
int sqlite3_bind_blob64(sqlite3_stmt *_pStmt, int _idx, const void *_val, sqlite3_uint64 _valSize, void(*_callback)(void*));
int sqlite3_bind_double(sqlite3_stmt *_pStmt, int _idx, double _val);
int sqlite3_bind_int(sqlite3_stmt *_pStmt, int _idx, int _val);
int sqlite3_bind_int64(sqlite3_stmt *_pStmt, int _idx, sqlite3_int64 _val);
int sqlite3_bind_null(sqlite3_stmt *_pStmt, int _idx);
int sqlite3_bind_text(sqlite3_stmt *_pStmt,int _idx,const char *_val, int _valSize,void(*_callback)(void*));
int sqlite3_bind_text16(sqlite3_stmt *_pStmt, int _idx, const void *_val, int _valSize, void(*_callback)(void*));
int sqlite3_bind_text64(sqlite3_stmt *_pStmt, int _idx, const char *_val, sqlite3_uint64 _valSize, void(*_callback)(void*), unsigned char _encoding);
int sqlite3_bind_value(sqlite3_stmt *_pStmt, int _idx, const sqlite3_value *_val);
int sqlite3_bind_pointer(sqlite3_stmt *_pStmt, int _idx, void *_val, const char *_type, void(*_callback)(void*));
int sqlite3_bind_zeroblob(sqlite3_stmt *_pStmt, int _idx, int _valSize);
int sqlite3_bind_zeroblob64(sqlite3_stmt *_pStmt, int _idx, sqlite3_uint64 _valSize);



/* -----------------------------------------------------------
 * -- 传入 sql语句中的 变量名，获得其 idx值，
 *    可用于 sqlite3_bind* 系列函数
 * 
 * -- 如果没有找到目标，返回0
 * 
 * -- 参数zName 必须为 UTF-8 格式。
 *    如果在 sql语句中，变量为 ":fstArg"
 *    那么， 参数zName 也应写入 ":fstArg" （注意这里的 ：号）
 * 
 * -- : @ 两个通配符都可以使用，推荐使用 : 
 */
int sqlite3_bind_parameter_index(sqlite3_stmt*, const char *zName);


/* ===========================================================
 *                        step
 * -----------------------------------------------------------
 * -- 用于执行 前面 prepare函数创建的 sql语句。
 *    需要被调用一次或多次。
 * 
 * -- V3. 6.23.1之前的 老版本 sqlite，在每次调用 step函数之后，需要调用 sqlite3_reset()
 *    在新版本中，则不需要，step函数会 自动调用 reset函数。
 * 
 * -- return：
 *    函数将返回一个以下的结果来标识其执行结果：
 *    SQLITE_DONE:
 *        正确调用. sql 语句已经被成功地执行. 在调用 sqlite_reset() 之前, 
 *        当前预编译的语句不应该被 sqlite3_step() 再次调用.
 *    SQLITE_BUSY:
 *        忙碌. 数据库引擎无法锁定数据去完成其工作. 但可以多次尝试.
 *    SQLITE_ROW:
 *        查询时产生了结果. 此时可以通过相关的"数据访问函数(column access functions)"来取得数据. 
 *        sqlite3_step() 的再一次调用将取得下一条查询结果.
 *    SQLITE_ERROR:
 *        发生了错误. 此时可以通过 sqlite3_errmmsg() 取得相关的错误信息. 
 *        sqlite3_step() 不能被再次调用.
 *    SQLITE_MISUSE:
 *        不正确的库的使用. 该函数使用不当.
 * 
 * -- 注意，此函数的成功 rc 不是 SQLITE_OK，而是 SQLITE_DONE。
 * 
 * -- 为此函数设置 wrap函数 是不全面的，需要 按需设置
 *      
 */
int sqlite3_step(sqlite3_stmt*);



/* ===========================================================
 *                       reset
 * -----------------------------------------------------------
 * -- 用于重置一个 prepare sql语句对象 到它的初始状态，然后准备被重新执行
 * 
 * -- 所有 sql语句变量 使用 sqlite3_bind*() 绑定值，
 *    使用 sqlite3_clear_bindings() 重设这些绑定。
 * 
 * -- return:
 *    SQLITE_OK   -- 一切正常
 *    SQLITE_BUSY -- 表示暂时无法执行操作，
 *    SQLITE_DONE -- 表示操作执行完毕，
 *    SQLITE_ROW  -- 表示执行完毕 并且有返回（执行select语句时）
 *                   此时需要对查询结果进行处理，SQLITE3提供 sqlite3_column_* 系列函数。
 *                       
 */
int sqlite3_reset(sqlite3_stmt *pStmt);




/* ===========================================================
 *                     column
 * -----------------------------------------------------------
 * -- 参数 iCol 为列的序号，从0开始。如果返回值有多行，则可以再次调用sqlite3_step函数，
 *    然后由 sqlite3_column_* 函数取 得返回值。
 *    使用上述这些函数基本上可以完成对 SQLITE3数据库 的操作了.
 * 
 * -- 参数 iCol 指定这一行中的想要被返回的列的索引。
 *    最左边的一列的索引号是0，行的列数可以使用 sqlite3_colum_count() 获得。
 * 
 * -- 只有在 sqlite3_step() 返回 SQLITE_ROW 时，
 *    且在下一次 sqlite3_reset() ／ sqlite3_finalize() 被调用前
 *    才能调用 此组函数 来获取返回值。
 *    否则 行为是未定义的
 * 
 * 
 * -- sqlite3_column_type() 返回 datatype code。为以下值之一：
 *       SQLITE_INTEGER
 *       SQLITE_FLOAT
 *       SQLITE_TEXT
 *       SQLITE_BLOB
 *       SQLITE_NULL
 *    猜测是用来 探测目标 column 的数据类型的。
 * 
 * -- 如果用于 blob，text。
 *    应当先用 sqlite3_column_bytes 来查明 目标数据的 字节数
 *    注意，在用于 text时，此函数返回的字节数，不包含 尾后0。
 * 
 * -- Summary:
 *   sqlite3_column_blob	-->	BLOB result
 *   sqlite3_column_double	-->	REAL result
 *   sqlite3_column_int	    -->	32-bit INTEGER result
 *   sqlite3_column_int64	-->	64-bit INTEGER result
 *   sqlite3_column_text	-->	UTF-8 TEXT result
 *   sqlite3_column_text16	-->	UTF-16 TEXT result
 *   sqlite3_column_value	-->	The result as an "unprotected sqlite3_value" object.
 * 
 *   sqlite3_column_bytes	-->	Size of a BLOB or a UTF-8 TEXT result in bytes
 *   sqlite3_column_bytes16 --> Size of UTF-16 TEXT in bytes
 *   sqlite3_column_type	-->	Default datatype of the result           
 */
const void *sqlite3_column_blob(sqlite3_stmt*, int iCol);
double sqlite3_column_double(sqlite3_stmt*, int iCol);
int sqlite3_column_int(sqlite3_stmt*, int iCol);
sqlite3_int64 sqlite3_column_int64(sqlite3_stmt*, int iCol);
const unsigned char *sqlite3_column_text(sqlite3_stmt*, int iCol);
const void *sqlite3_column_text16(sqlite3_stmt*, int iCol);
sqlite3_value *sqlite3_column_value(sqlite3_stmt*, int iCol);

int sqlite3_column_bytes(sqlite3_stmt*, int iCol);
int sqlite3_column_bytes16(sqlite3_stmt*, int iCol);
int sqlite3_column_type(sqlite3_stmt*, int iCol);


/* -----------------------------------------------------------
 * -- 当之前通过 select语句获得 查询返回值后，通过此函数，来获得 返回值 列数 
 * -- 如果之前执行了 类似 update 的sql语句，那么  prepared statement 里是没有数据的
 *    此时调用本函数 将返回 0               
 */
int sqlite3_column_count(sqlite3_stmt *pStmt);


/* ===========================================================
 *                        finalize
 * -----------------------------------------------------------
 * -- 这个过程 销毁 前面被 sqlite3_prepare() 创建的准备语句，
 *    每个准备语句都必须使用这个函数去销毁以防止内存泄露。
 *    
 * -- 在空指针上调用这个函数没有什么影响，
 * 
 * -- 可以在语句的生命周期的任一时刻调用这个函数：
 *       在语句被执行前，
 *       一次或多次调用sqlite_reset之后，
 *       或者在sqlite3_step任何调用之后不管语句是否完成执行
 *                    
 */
int sqlite3_finalize(sqlite3_stmt *pStmt);










