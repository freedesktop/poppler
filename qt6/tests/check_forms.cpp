#include <QtTest/QtTest>

#include <poppler-qt6.h>
#include <poppler-form.h>
#include <poppler-private.h>
#include <Form.h>

class TestForms : public QObject
{
    Q_OBJECT
public:
    TestForms(QObject *parent = nullptr) : QObject(parent) { }
private slots:
    void testCheckbox(); // Test for issue #655
    void testCheckboxIssue159(); // Test for issue #159
    void testSetIcon(); // Test that setIcon will always be valid.
    void testSetPrintable();
    void testSetAppearanceText();
    void testStandAloneWidgets(); // check for 'de facto' tooltips. Issue #34
    void testUnicodeFieldAttributes();
};

void TestForms::testCheckbox()
{
    // Test for checkbox issue #655
    QScopedPointer<Poppler::Document> document(Poppler::Document::load(TESTDATADIR "/unittestcases/latex-hyperref-checkbox-issue-655.pdf"));
    QVERIFY(document);

    QScopedPointer<Poppler::Page> page(document->page(0));
    QVERIFY(page);

    QList<Poppler::FormField *> forms = page->formFields();
    QCOMPARE(forms.size(), 1);

    Poppler::FormField *form = forms.at(0);
    QCOMPARE(form->type(), Poppler::FormField::FormButton);

    Poppler::FormFieldButton *chkFormFieldButton = static_cast<Poppler::FormFieldButton *>(form);

    // Test this is actually a Checkbox
    QCOMPARE(chkFormFieldButton->buttonType(), Poppler::FormFieldButton::CheckBox);

    // checkbox comes initially 'unchecked'
    QCOMPARE(chkFormFieldButton->state(), false);
    // let's mark it as 'checked'
    chkFormFieldButton->setState(true);
    // now test if it was successfully 'checked'
    QCOMPARE(chkFormFieldButton->state(), true);
}

void TestForms::testStandAloneWidgets()
{
    // Check for 'de facto' tooltips. Issue #34
    QScopedPointer<Poppler::Document> document(Poppler::Document::load(TESTDATADIR "/unittestcases/tooltip.pdf"));
    QVERIFY(document);

    QScopedPointer<Poppler::Page> page(document->page(0));
    QVERIFY(page);

    QList<Poppler::FormField *> forms = page->formFields();

    QCOMPARE(forms.size(), 3);

    Q_FOREACH (Poppler::FormField *field, forms) {
        QCOMPARE(field->type(), Poppler::FormField::FormButton);

        Poppler::FormFieldButton *fieldButton = static_cast<Poppler::FormFieldButton *>(field);
        QCOMPARE(fieldButton->buttonType(), Poppler::FormFieldButton::Push);

        FormField *ff = Poppler::FormFieldData::getFormWidget(fieldButton)->getField();
        QVERIFY(ff);
        QCOMPARE(ff->isStandAlone(), true);

        // tooltip.pdf has only these 3 standalone widgets
        QVERIFY(field->uiName() == QStringLiteral("This is a tooltip!") || // clazy:exclude=qstring-allocations
                field->uiName() == QStringLiteral("Sulfuric acid") || field->uiName() == QString::fromUtf8("little Gauß"));
    }
}

void TestForms::testCheckboxIssue159()
{
    // Test for checkbox issue #159
    QScopedPointer<Poppler::Document> document(Poppler::Document::load(TESTDATADIR "/unittestcases/checkbox_issue_159.pdf"));
    QVERIFY(document);

    QScopedPointer<Poppler::Page> page(document->page(0));
    QVERIFY(page);

    Poppler::FormFieldButton *beerFieldButton = nullptr;
    Poppler::FormFieldButton *wineFieldButton = nullptr;

    QList<Poppler::FormField *> forms = page->formFields();

    // Let's find and assign the "Wine" and "Beer" radio buttons
    Q_FOREACH (Poppler::FormField *field, forms) {
        if (field->type() != Poppler::FormField::FormButton)
            continue;

        Poppler::FormFieldButton *fieldButton = static_cast<Poppler::FormFieldButton *>(field);
        if (fieldButton->buttonType() != Poppler::FormFieldButton::Radio)
            continue;

        // printf("%s \n", fieldButton->caption().toLatin1().data());
        if (fieldButton->caption() == QStringLiteral("Wine")) {
            wineFieldButton = fieldButton;
        } else if (fieldButton->caption() == QStringLiteral("Beer")) {
            beerFieldButton = fieldButton;
        }
    }

    // "Beer" and "Wine" radiobuttons belong to the same RadioButton group.
    // So selecting one should unselect the other.
    QVERIFY(beerFieldButton);
    QVERIFY(wineFieldButton);

    // Test that the RadioButton group comes with "Beer" initially selected
    QCOMPARE(beerFieldButton->state(), true);

    // Now select "Wine". As a result "Beer" should no longer be selected.
    wineFieldButton->setState(true);

    // Test that "Beer" is indeed not reporting as being selected
    QCOMPARE(beerFieldButton->state(), false);
}

void TestForms::testSetIcon()
{
    QScopedPointer<Poppler::Document> document(Poppler::Document::load(TESTDATADIR "/unittestcases/form_set_icon.pdf"));
    QVERIFY(document);

    QScopedPointer<Poppler::Page> page(document->page(0));
    QVERIFY(page);

    QList<Poppler::FormField *> forms = page->formFields();

    Poppler::FormFieldButton *anmButton = nullptr;

    // First we are finding the field which will have its icon changed
    Q_FOREACH (Poppler::FormField *field, forms) {

        if (field->type() != Poppler::FormField::FormButton)
            continue;

        Poppler::FormFieldButton *fieldButton = static_cast<Poppler::FormFieldButton *>(field);
        if (field->name() == QStringLiteral("anm0"))
            anmButton = fieldButton;
    }

    QVERIFY(anmButton);

    // Then we set the Icon on this field, for every other field
    // And verify if it has a valid icon
    Q_FOREACH (Poppler::FormField *field, forms) {

        if (field->type() != Poppler::FormField::FormButton)
            continue;

        Poppler::FormFieldButton *fieldButton = static_cast<Poppler::FormFieldButton *>(field);
        if (field->name() == QStringLiteral("anm0"))
            continue;

        Poppler::FormFieldIcon newIcon = fieldButton->icon();

        anmButton->setIcon(newIcon);

        Poppler::FormFieldIcon anmIcon = anmButton->icon();

        QVERIFY(Poppler::FormFieldIconData::getData(anmIcon));
        QVERIFY(Poppler::FormFieldIconData::getData(anmIcon)->icon);

        QCOMPARE(Poppler::FormFieldIconData::getData(anmIcon)->icon->lookupNF("AP").dictLookupNF("N").getRef().num, Poppler::FormFieldIconData::getData(newIcon)->icon->lookupNF("AP").dictLookupNF("N").getRef().num);
    }

    // Just making sure that setting a invalid icon will still produce a valid icon.
    anmButton->setIcon(nullptr);
    Poppler::FormFieldIcon anmIcon = anmButton->icon();

    QVERIFY(Poppler::FormFieldIconData::getData(anmIcon));
    QVERIFY(Poppler::FormFieldIconData::getData(anmIcon)->icon);
}

void TestForms::testSetPrintable()
{
    QScopedPointer<Poppler::Document> document(Poppler::Document::load(TESTDATADIR "/unittestcases/form_set_icon.pdf"));
    QVERIFY(document);

    QScopedPointer<Poppler::Page> page(document->page(0));
    QVERIFY(page);

    QList<Poppler::FormField *> forms = page->formFields();

    Q_FOREACH (Poppler::FormField *field, forms) {
        field->setPrintable(true);
        QCOMPARE(field->isPrintable(), true);

        field->setPrintable(false);
        QCOMPARE(field->isPrintable(), false);
    }
}

void TestForms::testSetAppearanceText()
{
    QScopedPointer<Poppler::Document> document(Poppler::Document::load(TESTDATADIR "/unittestcases/checkbox_issue_159.pdf"));
    QVERIFY(document);

    QScopedPointer<Poppler::Page> page(document->page(0));
    QVERIFY(page);

    QList<Poppler::FormField *> forms = page->formFields();

    int nTextForms = 0;

    Q_FOREACH (Poppler::FormField *field, forms) {

        if (field->type() != Poppler::FormField::FormText)
            continue;

        nTextForms++;

        Poppler::FormFieldText *fft = static_cast<Poppler::FormFieldText *>(field);

        const QString textToSet = "HOLA" + fft->name();
        fft->setAppearanceText(textToSet);

        Dict *dict = Poppler::FormFieldData::getFormWidget(fft)->getObj()->getDict();
        Object strObject = dict->lookup("AP").dictLookup("N");

        QVERIFY(strObject.isStream());

        GooString s;
        strObject.getStream()->fillGooString(&s);

        const QString textToFind = QStringLiteral("\n(%1) Tj\n").arg(textToSet);
        QVERIFY(s.toStr().find(textToFind.toStdString()) != std::string::npos);
    }

    QCOMPARE(nTextForms, 5);
}

void TestForms::testUnicodeFieldAttributes()
{
    QScopedPointer<Poppler::Document> document(Poppler::Document::load(TESTDATADIR "/unittestcases/fieldWithUtf16Names.pdf"));
    QVERIFY(document);

    QScopedPointer<Poppler::Page> page(document->page(0));
    QVERIFY(page);

    QList<Poppler::FormField *> forms = page->formFields();

    Poppler::FormField *field = forms.first();

    QCOMPARE(field->name(), QStringLiteral("Tex"));
    QCOMPARE(field->uiName(), QStringLiteral("Texto de ayuda"));
}

QTEST_GUILESS_MAIN(TestForms)
#include "check_forms.moc"
