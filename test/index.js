'use strict';

const expect = require('chai').expect;
const MeCab = require('../index');

describe('node-mecab-native', () => {
  it('should parse text', (done) => {
    var tagger = MeCab.Tagger();
    tagger.parse('太郎は次郎が持っている本を花子に渡した。', (err, res) => {
      let expected =
        '太郎\t名詞,固有名詞,人名,名,*,*,太郎,タロウ,タロー\n' +
        'は\t助詞,係助詞,*,*,*,*,は,ハ,ワ\n' +
        '次郎\t名詞,固有名詞,人名,名,*,*,次郎,ジロウ,ジロー\n' +
        'が\t助詞,格助詞,一般,*,*,*,が,ガ,ガ\n' +
        '持っ\t動詞,自立,*,*,五段・タ行,連用タ接続,持つ,モッ,モッ\n' +
        'て\t助詞,接続助詞,*,*,*,*,て,テ,テ\n' +
        'いる\t動詞,非自立,*,*,一段,基本形,いる,イル,イル\n' +
        '本\t名詞,一般,*,*,*,*,本,ホン,ホン\n' +
        'を\t助詞,格助詞,一般,*,*,*,を,ヲ,ヲ\n' +
        '花\t名詞,一般,*,*,*,*,花,ハナ,ハナ\n' +
        '子\t名詞,接尾,一般,*,*,*,子,コ,コ\n' +
        'に\t助詞,格助詞,一般,*,*,*,に,ニ,ニ\n' +
        '渡し\t動詞,自立,*,*,五段・サ行,連用形,渡す,ワタシ,ワタシ\n' +
        'た\t助動詞,*,*,*,特殊・タ,基本形,た,タ,タ\n' +
        '。\t記号,句点,*,*,*,*,。,。,。\n' +
        'EOS\n';
      expect(res).to.be.equal(expected);

      done();
    });
  });

  it('should parse text by wakati', (done) => {
    var tagger = MeCab.Tagger('-Owakati');
    tagger.parse('太郎は次郎が持っている本を花子に渡した。', (err, res) => {
      expect(res).to.be.equal('太郎 は 次郎 が 持っ て いる 本 を 花 子 に 渡し た 。 \n');

      done();
    });
  });

  it('should parse text', (done) => {
    var tagger = MeCab.Tagger();
    tagger.parseToNode('太郎は次郎が持っている本を花子に渡した。', (err, res) => {
      expect(res).to.be.instanceof(Array);
      expect(res).to.have.lengthOf(17);
      res.forEach(item => expect(item).to.have.all.keys([
        'word',
        'id',
        'surface',
        'feature',
        'len',
        'rcAttr',
        'lcAttr',
        'posid',
        'char_type',
        'stat',
        'isbest',
        'alpha',
        'beta',
        'prob',
        'cost'
      ]));

      done();
    });
  });

  it('should parse text', (done) => {
    var tagger = MeCab.Tagger();
    tagger.dictionaryInfo((err, res) => {
      expect(res).to.be.instanceof(Array);
      expect(res).to.have.lengthOf(1);
      res.forEach(item => expect(item).to.have.all.keys([
        'charset',
        'filename',
        'lsize',
        'rsize',
        'size',
        'type',
        'version'
      ]));

      done();
    });
  });
});
