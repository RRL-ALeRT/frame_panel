#ifndef FRAME_PANEL_HPP
#define FRAME_PANEL_HPP

#include <rviz_common/panel.hpp>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

#include <QLineEdit>
#include <QPushButton>

namespace my_frame_panel
{

class FramePanel : public rviz_common::Panel
{
Q_OBJECT
public:
  explicit FramePanel(QWidget* parent = nullptr);

private Q_SLOTS:
  void onCreateClicked();

private:
  QLineEdit* name_edit_;
  QPushButton* create_button_;

  rclcpp::Node::SharedPtr node_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
};

}  // namespace my_frame_panel

#endif