#include "frame_panel/frame_panel.hpp"
#include <pluginlib/class_list_macros.hpp>
#include <QVBoxLayout>

namespace my_frame_panel
{

FramePanel::FramePanel(QWidget* parent)
: Panel(parent)
{
  node_ = rclcpp::Node::make_shared("frame_panel_node");

  publisher_ = node_->create_publisher<std_msgs::msg::String>(
    "/create_frame", 10);

  name_edit_ = new QLineEdit;
  name_edit_->setPlaceholderText("Enter frame name");

  create_button_ = new QPushButton("Create Frame");

  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(name_edit_);
  layout->addWidget(create_button_);
  setLayout(layout);

  connect(create_button_, SIGNAL(clicked()),
          this, SLOT(onCreateClicked()));
}

void FramePanel::onCreateClicked()
{
  std_msgs::msg::String msg;
  msg.data = name_edit_->text().toStdString();
  publisher_->publish(msg);
}

}  // namespace my_frame_panel

PLUGINLIB_EXPORT_CLASS(
  my_frame_panel::FramePanel,
  rviz_common::Panel)