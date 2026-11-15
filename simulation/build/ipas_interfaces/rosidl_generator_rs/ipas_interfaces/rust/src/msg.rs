#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to ipas_interfaces__msg__IPASMeasurement

// This struct is not documented.
#[allow(missing_docs)]

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct IPASMeasurement {

    // This member is not documented.
    #[allow(missing_docs)]
    pub temperature: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub humidity: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub pressure: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub gas_resistance: f32,

}



impl Default for IPASMeasurement {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::IPASMeasurement::default())
  }
}

impl rosidl_runtime_rs::Message for IPASMeasurement {
  type RmwMsg = super::msg::rmw::IPASMeasurement;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        temperature: msg.temperature,
        humidity: msg.humidity,
        pressure: msg.pressure,
        gas_resistance: msg.gas_resistance,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      temperature: msg.temperature,
      humidity: msg.humidity,
      pressure: msg.pressure,
      gas_resistance: msg.gas_resistance,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      temperature: msg.temperature,
      humidity: msg.humidity,
      pressure: msg.pressure,
      gas_resistance: msg.gas_resistance,
    }
  }
}


