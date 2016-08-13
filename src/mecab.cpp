#include <nan.h>
#include <mecab.h>
#include <vector>

class MeCab_Tagger : public Nan::ObjectWrap {
  public:
    static void Init(v8::Local<v8::Object> exports);

  private:
    MeCab_Tagger(const char *arg);
    ~MeCab_Tagger();

    static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void Parse(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void ParseToNode(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void DictionaryInfo(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static Nan::Persistent<v8::Function> constructor;

    MeCab::Tagger *tagger;
};

Nan::Persistent<v8::Function> MeCab_Tagger::constructor;

MeCab_Tagger::MeCab_Tagger(const char *option) {
  char *p = new char [strlen(option) + 4];
  strcpy(p, "-C ");
  strcat(p, option);
  tagger = MeCab::createTagger(p);
  delete [] p;
  if (! tagger) throw MeCab::getLastError();
}

MeCab_Tagger::~MeCab_Tagger() {
  delete tagger;
}

void MeCab_Tagger::Init(v8::Local<v8::Object> exports) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Tagger").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "parse", Parse);
  Nan::SetPrototypeMethod(tpl, "parseToNode", ParseToNode);
  Nan::SetPrototypeMethod(tpl, "dictionaryInfo", DictionaryInfo);

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("Tagger").ToLocalChecked(), tpl->GetFunction());
}

void MeCab_Tagger::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (info.IsConstructCall()) {
    v8::String::Utf8Value option(info[0]);

    MeCab_Tagger* mecabTagger = new MeCab_Tagger(*option);
    mecabTagger->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = { info[0] };
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void MeCab_Tagger::Parse(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  MeCab_Tagger* mecabTagger = ObjectWrap::Unwrap<MeCab_Tagger>(info.Holder());
  v8::String::Utf8Value input(info[0]);
  v8::Local<v8::Function> callback = info[1].As<v8::Function>();

  const char *result = mecabTagger->tagger->parse(*input);

  const unsigned argc = 2;
  v8::Local<v8::Value> argv[] = {
    Nan::Null(),
    Nan::New(result).ToLocalChecked()
  };
  Nan::MakeCallback(Nan::GetCurrentContext()->Global(), callback, argc, argv);
}

void MeCab_Tagger::ParseToNode(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  MeCab_Tagger* mecabTagger = ObjectWrap::Unwrap<MeCab_Tagger>(info.Holder());
  v8::String::Utf8Value input(info[0]);
  v8::Local<v8::Function> callback = info[1].As<v8::Function>();
  std::vector<v8::Local<v8::Object>> node_list;
  const MeCab::Node* node = mecabTagger->tagger->parseToNode(*input);

  for (; node; node = node->next) {
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    obj->Set(Nan::New("word").ToLocalChecked(), Nan::New(std::string(node->surface, node->length)).ToLocalChecked());
    obj->Set(Nan::New("id").ToLocalChecked(), Nan::New(node->id));
    obj->Set(Nan::New("surface").ToLocalChecked(), Nan::New(node->surface).ToLocalChecked());
    obj->Set(Nan::New("feature").ToLocalChecked(), Nan::New(node->feature).ToLocalChecked());
    obj->Set(Nan::New("len").ToLocalChecked(), Nan::New(node->length));
    obj->Set(Nan::New("rcAttr").ToLocalChecked(), Nan::New(node->rcAttr));
    obj->Set(Nan::New("lcAttr").ToLocalChecked(), Nan::New(node->lcAttr));
    obj->Set(Nan::New("posid").ToLocalChecked(), Nan::New(node->posid));
    obj->Set(Nan::New("char_type").ToLocalChecked(), Nan::New(node->char_type));
    obj->Set(Nan::New("stat").ToLocalChecked(), Nan::New(node->stat));
    obj->Set(Nan::New("isbest").ToLocalChecked(), Nan::New(node->isbest));
    obj->Set(Nan::New("alpha").ToLocalChecked(), Nan::New(node->alpha));
    obj->Set(Nan::New("beta").ToLocalChecked(), Nan::New(node->beta));
    obj->Set(Nan::New("prob").ToLocalChecked(), Nan::New(node->prob));
    obj->Set(Nan::New("cost").ToLocalChecked(), Nan::New((int)node->cost));
    node_list.push_back(obj);
  }

  v8::Local<v8::Array> results = Nan::New<v8::Array>(node_list.size());
  for(unsigned int i = 0; i < results->Length(); ++i) {
    results->Set(i, node_list.at(i));
  }

  const unsigned argc = 2;
  v8::Local<v8::Value> argv[] = {
    Nan::Null(),
    results
  };
  Nan::MakeCallback(Nan::GetCurrentContext()->Global(), callback, argc, argv);
}

void MeCab_Tagger::DictionaryInfo(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  MeCab_Tagger* obj = ObjectWrap::Unwrap<MeCab_Tagger>(info.Holder());
  v8::Local<v8::Function> callback = info[0].As<v8::Function>();
  const MeCab::DictionaryInfo *dic = obj->tagger->dictionary_info();
  std::vector<v8::Local<v8::Object>> info_list;

  for (; dic; dic = dic->next) {
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    obj->Set(Nan::New("filename").ToLocalChecked(), Nan::New(dic->filename).ToLocalChecked());
    obj->Set(Nan::New("charset").ToLocalChecked(), Nan::New(dic->charset).ToLocalChecked());
    obj->Set(Nan::New("size").ToLocalChecked(), Nan::New(dic->size));
    obj->Set(Nan::New("type").ToLocalChecked(), Nan::New(dic->type));
    obj->Set(Nan::New("lsize").ToLocalChecked(), Nan::New(dic->lsize));
    obj->Set(Nan::New("rsize").ToLocalChecked(), Nan::New(dic->rsize));
    obj->Set(Nan::New("version").ToLocalChecked(), Nan::New(dic->version));
    info_list.push_back(obj);
  }

  v8::Local<v8::Array> results = Nan::New<v8::Array>(info_list.size());
  for(unsigned int i = 0; i < results->Length(); ++i) {
    results->Set(i, info_list.at(i));
  }

  const unsigned argc = 2;
  v8::Local<v8::Value> argv[] = {
    Nan::Null(),
    results
  };
  Nan::MakeCallback(Nan::GetCurrentContext()->Global(), callback, argc, argv);
}

void init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  MeCab_Tagger::Init(exports);
}

NODE_MODULE(mecab, init)
