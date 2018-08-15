var fs = require('fs');
var saxon = require('saxonXslt');

  describe("XPath Processor foo", function() {
    var content;
    var saxonProcessor;

    before(function(done) {
//      yield setup();
        content = fs.readFileSync("./test/examples/query/books.xml", "utf8");
        saxonProcessor = new saxon.SaxonProcessor(true);
        console.dir("saxonProcessor "+saxonProcessor.version());
        saxonProcessor.setcwd(".");
        done();
    });

    it("xpath without namespace declaration", function(done) {
        try
        {
            var xpathProcessor = saxonProcessor.newXPathProcessor();
            //xpathProcessor.declareNamespace("cml", "http://www.xml-cml.org/schema");
            xpathProcessor.setContextFile("./test/examples/query/books.xml");
            var vp=xpathProcessor.evaluate("/BOOKLIST/BOOKS/ITEM/TITLE");
            vp.size().should.equal(6);
            console.dir(vp.size());
            var vi=vp.getHead();
            console.dir(vi.size());
            var title=vi.getHead();
            console.dir(title.getStringValue());
        }
        catch (err) {
        console.dir(err.message);
        }
        done();
    });

    it("xpath with namespace declaration", function(done) {
        try
        {
            var xpathProcessor = saxonProcessor.newXPathProcessor();
            xpathProcessor.declareNamespace("cml", "http://www.xml-cml.org/schema");
            xpathProcessor.setContextFile("./test/examples/xml/sodium-icosanoate.xml");
            var vp=xpathProcessor.evaluate("/cml:cml/cml:molecule/cml:molecule/cml:atomArray/cml:atom/@elementType");
            console.dir(vp);
        }
        catch (err) {
        console.dir(err.message);
        }
        done();
    });

    after(function(done) {
//        saxonProcessor.release();
//      yield teardown();
        done();
    });
  });
