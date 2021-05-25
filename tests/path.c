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
    AvmString ext = AvmPathGetExtension("/some/file.txt");

    assert(AvmObjectEquals(&ext, &expected));
}

void TestPathGetName()
{
    AvmString expected1 = AvmStringFrom("file.txt");
    AvmString expected2 = AvmStringFrom("name");
    AvmString name1 = AvmPathGetName("/some/dir/file.txt");
    AvmString name2 = AvmPathGetName("/some/dir/name");

    assert(AvmObjectEquals(&name1, &expected1));
    assert(AvmObjectEquals(&name2, &expected2));
}

void TestPathGetParent()
{
    AvmString expected1_2 = AvmStringFrom("/some/dir/");
    AvmString expected3_4 = AvmStringFrom("/");
    AvmString name1 = AvmPathGetParent("/some/dir/probably");
    AvmString name2 = AvmPathGetParent("/some/dir/probably.or.not");
    AvmString name3 = AvmPathGetParent("/dir");
    AvmString name4 = AvmPathGetParent("/");

    assert(AvmObjectEquals(&name1, &expected1_2));
    assert(AvmObjectEquals(&name2, &expected1_2));
    assert(AvmObjectEquals(&name3, &expected3_4));
    assert(AvmObjectEquals(&name4, &expected3_4));
}

void main()
{
    TestPathCombine2();
    TestPathCombine();
    TestPathGetExtension();
    TestPathGetName();
    TestPathGetParent();
}
