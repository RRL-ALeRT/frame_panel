#ifndef FRAME_PANEL_HPP
#define FRAME_PANEL_HPP

#include <rviz_common/panel.hpp>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QStringList>
#include <QTimer>

namespace my_frame_panel
{

class FramePanel : public rviz_common::Panel
{
Q_OBJECT
public:
  explicit FramePanel(QWidget* parent = nullptr);

Q_SIGNALS:
  void framesListUpdated(const QStringList& frames);

private Q_SLOTS:
  void onAddClicked();
  void onRemoveClicked();
  void onRenameClicked();
  void updateComboBox(const QStringList& frames);

private:
  void activeFramesCallback(const std_msgs::msg::String::SharedPtr msg);

  QComboBox* frame_combo_;
  QLineEdit* name_edit_;
  QLineEdit* reference_orientation_name;
  QPushButton* add_button_;
  QPushButton* remove_button_;
  QPushButton* rename_button_;
  QTimer* spin_timer_;

  rclcpp::Node::SharedPtr node_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_create_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_delete_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_rename_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_active_frames_;
};

}  // namespace my_frame_panel

#endif