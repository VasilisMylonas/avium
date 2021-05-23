#include "avium/path.h"
#include "avium/string.h"
#include "avium/testing.h"

void TestPathCombine2()
{
    AvmString expected = AvmStringFrom("/some/path");
    AvmString a = AvmPathCombine2("/some", "/path");
    AvmString b = AvmPathCombine2("/some", "./path");
    AvmString c = AvmPathCombine2("/some", "path");
    AvmString d = AvmPathCombine2("/some/", "/path");
    AvmString e = AvmPathCombine2("/some/", "./path");
    AvmString f = AvmPathCombine2("/some/", "path");

    assert(AvmObjectEquals(&a, &b));
    assert(AvmObjectEquals(&a, &c));
    assert(AvmObjectEquals(&a, &d));
    assert(AvmObjectEquals(&a, &e));
    assert(AvmObjectEquals(&a, &f));
    assert(AvmObjectEquals(&a, &expected));
}

void TestPathCombine3()
{
    AvmString expected = AvmStringFrom("/some/other/path");
    AvmString a = AvmPathCombine3("/some", "/other", "/path");
    AvmString b = AvmPathCombine3("/some", "/other", "./path");
    AvmString c = AvmPathCombine3("/some", "/other", "path");
    AvmString d = AvmPathCombine3("/some/", "/other", "/path");
    AvmString e = AvmPathCombine3("/some/", "/other", "./path");
    AvmString f = AvmPathCombine3("/some/", "/other", "path");
    AvmString g = AvmPathCombine3("/some", "/other/", "/path");
    AvmString h = AvmPathCombine3("/some", "/other/", "./path");
    AvmString i = AvmPathCombine3("/some", "/other/", "path");
    AvmString j = AvmPathCombine3("/some", "other", "/path");
    AvmString k = AvmPathCombine3("/some", "other", "./path");
    AvmString l = AvmPathCombine3("/some", "other", "path");
    AvmString m = AvmPathCombine3("/some", "./other", "/path");
    AvmString n = AvmPathCombine3("/some", "./other", "./path");
    AvmString o = AvmPathCombine3("/some", "./other", "path");

    assert(AvmObjectEquals(&a, &b));
    assert(AvmObjectEquals(&a, &c));
    assert(AvmObjectEquals(&a, &d));
    assert(AvmObjectEquals(&a, &e));
    assert(AvmObjectEquals(&a, &f));
    assert(AvmObjectEquals(&a, &g));
    assert(AvmObjectEquals(&a, &h));
    assert(AvmObjectEquals(&a, &i));
    assert(AvmObjectEquals(&a, &j));
    assert(AvmObjectEquals(&a, &k));
    assert(AvmObjectEquals(&a, &l));
    assert(AvmObjectEquals(&a, &m));
    assert(AvmObjectEquals(&a, &n));
    assert(AvmObjectEquals(&a, &o));
    assert(AvmObjectEquals(&a, &expected));
}

void TestPathCombine()
{
    AvmString expected1 = AvmStringFrom("/some/other/path");
    AvmString expected2 = AvmStringFrom("/some/path");
    AvmString expected3 = AvmStringFrom("/some/other/big/path");
    AvmString a = AvmPathCombineV("/some", "/other", "/path");
    AvmString b = AvmPathCombineV("/some", "./path");
    AvmString c = AvmPathCombineV("/some", "/other", "big", "path");

    assert(AvmObjectEquals(&a, &expected1));
    assert(AvmObjectEquals(&b, &expected2));
    assert(AvmObjectEquals(&c, &expected3));
}

void TestPathGetExtension()
{
    AvmString expected = AvmStringFrom("txt");
    AvmString ext = AvmPathGetExtension("/some.f/file.txt");

    assert(AvmObjectEquals(&ext, &expected));
}

void TestPathGetDirName()
{
    AvmString expected = AvmStringFrom("/some/dir/");
    AvmString name = AvmPathGetDirName("/some/dir/name");

    assert(AvmObjectEquals(&name, &expected));
}

void TestPathGetFileName()
{
    AvmString expected = AvmStringFrom("file.txt");
    AvmString name = AvmPathGetFileName("/some/dir/file.txt");

    assert(AvmObjectEquals(&name, &expected));
}

void main()
{
    TestPathCombine2();
    TestPathCombine3();
    TestPathCombine();
    TestPathGetExtension();
    TestPathGetDirName();
    TestPathGetFileName();
}
