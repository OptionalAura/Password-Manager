#pragma once
#include "PMLineEdit.h"
#include "../resources/pm.h"

/**
 * @fn	PMLineEdit::PMLineEdit(QWidget* parent)
 *
 * @brief	Constructor
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 *
 * @param [in,out]	parent	If non-null, the parent.
 */
PMLineEdit::PMLineEdit(QWidget* parent) : QLineEdit(parent)
{
	this->installEventFilter(this);
	SetQssProperty("border-radius", QString("3px"));
	SetQssProperty("font", "10pt \"Segoe UI\"");
	this->AutoSetColorBasedOn(QColor("#333333"));
	this->baseBorderColor = QColor("#333333");
	this->hoverBorderColor = QColor("#515151");
	this->focusedBorderColor = QColor("#2962FF");
	LOG(QString("Border colors: %1, %2, %3").arg(baseBorderColor.name(QColor::NameFormat::HexRgb)).arg(hoverBorderColor.name(QColor::NameFormat::HexRgb)).arg(baseBorderColor.name(QColor::NameFormat::HexRgb)).toStdString());
	this->SetBorderColor(this->baseBorderColor);
	this->repaint();
	LOG(styleSheet().toStdString());
}

/**
 * @fn	void PMLineEdit::SetColor(const QColor& color)
 *
 * @brief	Sets the background color
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 *
 * @param 	color	The color.
 */
void PMLineEdit::SetColor(const QColor& color)
{
	this->backgroundcolor = color;
	QString qss;
	SetQssProperty("background-color", QString("rgb(%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue()));
	updateStyleSheet();
}

/**
 * @fn	void PMLineEdit::updateStyleSheet()
 *
 * @brief	Updates the style sheet
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 */
void PMLineEdit::updateStyleSheet() {
	setStyleSheet(GetStyleSheet());
}

/**
 * @fn	const QString PMLineEdit::GetStyleSheet()
 *
 * @brief	Gets the style sheet
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 *
 * @returns	The style sheet.
 */
const QString PMLineEdit::GetStyleSheet() {
	QString format("QLineEdit{");
	for (std::pair<QString, QString> pair : qss) {
		format.append(pair.first).append(":").append(pair.second).append(";");
	}
	format.append("}");
	return format;
}

/**
 * @fn	void PMLineEdit::SetQssProperty(QString prop, QString value)
 *
 * @brief	Sets a qss property
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 *
 * @param 	prop 	The property.
 * @param 	value	The value.
 */
void PMLineEdit::SetQssProperty(QString prop, QString value) {
	qss.insert_or_assign(prop, value);
}

/**
 * @fn	const QString PMLineEdit::GetQssProperty(QString prop) const
 *
 * @brief	Gets a qss property
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 *
 * @param 	prop	The property.
 *
 * @returns	The qss property.
 */
const QString PMLineEdit::GetQssProperty(QString prop) const {
	return qss.at(prop);
}

/**
 * @fn	void PMLineEdit::SetFontColor(const QColor& color)
 *
 * @brief	Sets the font color
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 *
 * @param 	color	The color.
 */
void PMLineEdit::SetFontColor(const QColor& color)
{
	SetQssProperty("color", QString("rgba(%1, %2, %3, %4)").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
}

/**
 * @fn	const QColor& PMLineEdit::GetColor() const noexcept
 *
 * @brief	Gets the background color
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 *
 * @returns	The color.
 */
const QColor& PMLineEdit::GetColor() const noexcept
{
	return backgroundcolor;
}

/**
 * @fn	void PMLineEdit::SetBorderColor(const QColor& color)
 *
 * @brief	Sets the border color
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 *
 * @param 	color	The color.
 */
void PMLineEdit::SetBorderColor(const QColor& color)
{
	this->bordercolor = color;
	QString qss;
	SetQssProperty("border", QString("1px solid rgb(%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue()));
	updateStyleSheet();
}

/**
 * @fn	const QColor& PMLineEdit::GetBorderColor() const noexcept
 *
 * @brief	Gets the border color
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 *
 * @returns	The border color.
 */
const QColor& PMLineEdit::GetBorderColor() const noexcept {
	return this->bordercolor;
}

/**
 * @fn	void PMLineEdit::AutoSetColorBasedOn(const QColor& color)
 *
 * @brief	Automatically set the colors based on another color
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 *
 * @param 	color	The color.
 */
void PMLineEdit::AutoSetColorBasedOn(const QColor& color) {
	LOG("Setting color");
	this->SetColor(color);
	this->baseColor = color;
	this->hoverColor = color.darker(140);
	this->pushColor = color.darker(200);
	this->SetFontColor(pm::getOptimalFontColor(color));
	updateStyleSheet();
}

/**
 * @fn	bool PMLineEdit::eventFilter(QObject* obj, QEvent* e)
 *
 * @brief	Handles QEvents on this object
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 *
 * @param [in,out]	obj	If non-null, the object.
 * @param [in,out]	e  	If non-null, a QEvent to process.
 *
 * @returns	True if it succeeds, false if it fails.
 */
bool PMLineEdit::eventFilter(QObject* obj, QEvent* e)
{
	const QEvent::Type type = e->type();
	if (type == QEvent::FocusIn) {
		StartFocusAnimation();
	}
	else if (type == QEvent::FocusOut) {
		StopFocusAnimation();
	}
	else if (type == QEvent::MouseButtonPress) {
		LOG("Mouse down");
		if (!this->hasFocus())
			StartMousepressAnimation();
	}
	else if (type == QEvent::MouseButtonRelease) {
		LOG("Mouse up");
		if (!this->hasFocus())
			StopMousepressAnimation();
	}
	else if (type == QEvent::Enter) {
		LOG("Mouse over");
		if (!this->hasFocus())
			StartHoverEnterAnimation();
		hovering = true;
	}
	else if (type == QEvent::Leave) {
		LOG("Mouse out");
		if (!this->hasFocus())
			StopHoverEnterAnimation();
		hovering = false;
	}
	return false;
}

/**
 * @fn	void PMLineEdit::StartFocusAnimation()
 *
 * @brief	Starts focus animation
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 */
void PMLineEdit::StartFocusAnimation() {
	auto animation = new QPropertyAnimation(this, "bordercolor");
	animation->setDuration(200);
	animation->setStartValue(hovering ? hoverBorderColor : baseBorderColor);
	animation->setEndValue(focusedBorderColor);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}

/**
 * @fn	void PMLineEdit::StopFocusAnimation()
 *
 * @brief	Stops focus animation
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 */
void PMLineEdit::StopFocusAnimation() {
	auto animation = new QPropertyAnimation(this, "bordercolor");
	animation->setDuration(200);
	animation->setStartValue(focusedBorderColor);
	animation->setEndValue(hovering ? hoverBorderColor : baseBorderColor);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}

/**
 * @fn	void PMLineEdit::StartMousepressAnimation()
 *
 * @brief	Starts mousepress animation
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 */
void PMLineEdit::StartMousepressAnimation() {
	auto animation = new QPropertyAnimation(this, "backgroundcolor");
	animation->setDuration(50);
	animation->setStartValue(hovering ? hoverBorderColor : baseBorderColor);
	animation->setEndValue(pushColor);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}

/**
 * @fn	void PMLineEdit::StopMousepressAnimation()
 *
 * @brief	Stops mousepress animation
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 */
void PMLineEdit::StopMousepressAnimation()
{
	auto animation = new QPropertyAnimation(this, "backgroundcolor");
	animation->setDuration(200);
	animation->setStartValue(pushColor);
	animation->setEndValue(hoverColor);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}

/**
 * @fn	void PMLineEdit::StartHoverEnterAnimation()
 *
 * @brief	Starts hover enter animation
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 */
void PMLineEdit::StartHoverEnterAnimation()
{
	auto animation = new QPropertyAnimation(this, "bordercolor");
	animation->setDuration(200);
	animation->setStartValue(baseBorderColor);
	animation->setEndValue(hoverBorderColor);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}

/**
 * @fn	void PMLineEdit::StopHoverEnterAnimation()
 *
 * @brief	Stops hover enter animation
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 */
void PMLineEdit::StopHoverEnterAnimation()
{
	auto animation = new QPropertyAnimation(this, "bordercolor");
	animation->setDuration(200);
	animation->setStartValue(hoverBorderColor);
	animation->setEndValue(baseBorderColor);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}