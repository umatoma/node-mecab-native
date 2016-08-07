node-mecab-native
-----
MeCab Node.js binding

Requirements
-----
- MeCab (mecab, mecab-config, mecab-ipadic)
- Node.js 4.x
- Linux OS

Install MeCab
-----
```bash
$ sudo yum install http://packages.groonga.org/centos/groonga-release-1.1.0-1.noarch.rpm
$ sudo yum install mecab mecab-ipadic mecab-config
```

Install
-----
```bash
$ npm install node-mecab-native
```

Usage
-----
```javascript
'use strict';

var MeCab = require('node-mecab-native');
var tagger = MeCab.Tagger('-Owakati');

// Parse text
tagger.parse('太郎は次郎が持っている本を花子に渡した。', function(err, res) {
  console.log(res);
});

// Parse text to Nodes
tagger.parseToNode('太郎は次郎が持っている本を花子に渡した。', function(err, res) {
  console.log(res);
});

// Get Dictionary Info
tagger.dictionaryInfo(function(err, res) {
  console.log(res);
});
```

License
-----
MIT
