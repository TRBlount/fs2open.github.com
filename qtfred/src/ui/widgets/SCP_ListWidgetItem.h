#pragma once

#include <QListWidgetItem>

namespace fso {
namespace fred {

template<typename T>
class SCP_ListWidgetItem : public QListWidgetItem {
  public:
	  SCP_ListWidgetItem(QListWidget* parent = nullptr);
	  SCP_ListWidgetItem(const QString& text, QListWidget* parent = nullptr);

	  T getUserData() const;
	  void setUserData(const T data);

  private:
	  T userData;
};

template <typename T>
inline SCP_ListWidgetItem<T>::SCP_ListWidgetItem(QListWidget* parent) : QListWidgetItem(parent, QListWidgetItem::UserType)
{
}

template <typename T>
inline SCP_ListWidgetItem<T>::SCP_ListWidgetItem(const QString& text, QListWidget* parent)
	: QListWidgetItem(text, parent, QListWidgetItem::UserType)
{
}

template <typename T>
inline T SCP_ListWidgetItem<T>::getUserData() const
{
	return userData;
}

template <typename T>
inline void SCP_ListWidgetItem<T>::setUserData(const T data)
{
	this->userData = data;
}

} // namespace fred
} // namespace fso
