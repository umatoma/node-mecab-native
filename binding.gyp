# $ sudo yum install http://packages.groonga.org/centos/groonga-release-1.1.0-1.noarch.rpm
# $ sudo yum install mecab mecab-ipadic mecab-config
{
    "targets": [
        {
            "target_name": "mecab",
            "sources": ["mecab.cpp"],
            "include_dirs": [
                "<!(node -e \"require('nan')\")",
                "<!(mecab-config --inc-dir)"
            ],
            "libraries": ["<!(mecab-config --libs)"],
            "cflags_cc": ["-fexceptions"]
        }
    ]
}
