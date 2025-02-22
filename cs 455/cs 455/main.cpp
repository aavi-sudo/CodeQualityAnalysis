#include <Wt/WApplication.h>
#include <Wt/WLineEdit.h>
#include <Wt/WText.h>
#include <Wt/WPushButton.h>
#include <Wt/WSpinBox.h>
#include <Wt/WValidator.h>
#include <Wt/WBreak.h>
#include <string>

class TipCalcApplication : public Wt::Application {
public:
  explicit TipCalcApplication(const Wt::WEnvironment& env);

private:
  Wt::WLineEdit* mealAmount;
  Wt::WSpinBox* tipPercentage;
  Wt::WText* tipAmountText;
  Wt::WPushButton* calculateButton;

  void calculateTip();
};

TipCalcApplication::TipCalcApplication(const Wt::WEnvironment& env)
  : Wt::Application(env) {
  setTitle("Tip Calculator");

  // Meal amount input
  root()->addWidget(std::make_unique<Wt::WText>("Meal Amount: $"));
  mealAmount = root()->addWidget(std::make_unique<Wt::WLineEdit>());
  mealAmount->setValidator(Wt::RegExpValidator(R"(\d{0,3}(\.\d{2})?)")); // Restrict to 0-999.99

  // Tip percentage spinner
  root()->addWidget(std::make_unique<Wt::WText>("Tip (%): "));
  tipPercentage = root()->addWidget(std::make_unique<Wt::WSpinBox>());
  tipPercentage->setValue(10); // Initial value
  tipPercentage->setMin(10);
  tipPercentage->setMax(35);
  tipPercentage->setStep(5); // Increment by 5%

  // Calculate button
  calculateButton = root()->addWidget(std::make_unique<Wt::WPushButton>("Calculate Tip"));
  root()->addWidget(std::make_unique<Wt::WBreak>());

  // Tip amount display
  tipAmountText = root()->addWidget(std::make_unique<Wt::WText>("Tip Amount: $"));
  tipAmountText->setText("0.00");

  // Connect button click to calculateTip function
  calculateButton->clicked().connect(this, &TipCalcApplication::calculateTip);
}

void TipCalcApplication::calculateTip() {
  // Get values and convert to numbers
  double meal = std::stod(mealAmount->text().toUTF8());
  int tipPercent = tipPercentage->value();

  // Calculate tip amount
  double tip = meal * (tipPercent / 100.0);

  // Display tip amount with two decimal places
  tipAmountText->setText(std::to_string(tip).substr(0, tip.find(".") + 3));
}

int main(int argc, char** argv) {
  return Wt::WRun(argc, argv, [](const Wt::Environment& env) {
    return std::make_unique<TipCalcApplication>(env);
  });
}
