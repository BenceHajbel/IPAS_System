#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "ipas_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ipas_interfaces__msg__IPASMeasurement() -> *const std::ffi::c_void;
}

#[link(name = "ipas_interfaces__rosidl_generator_c")]
extern "C" {
    fn ipas_interfaces__msg__IPASMeasurement__init(msg: *mut IPASMeasurement) -> bool;
    fn ipas_interfaces__msg__IPASMeasurement__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<IPASMeasurement>, size: usize) -> bool;
    fn ipas_interfaces__msg__IPASMeasurement__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<IPASMeasurement>);
    fn ipas_interfaces__msg__IPASMeasurement__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<IPASMeasurement>, out_seq: *mut rosidl_runtime_rs::Sequence<IPASMeasurement>) -> bool;
}

// Corresponds to ipas_interfaces__msg__IPASMeasurement
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
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
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ipas_interfaces__msg__IPASMeasurement__init(&mut msg as *mut _) {
        panic!("Call to ipas_interfaces__msg__IPASMeasurement__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for IPASMeasurement {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ipas_interfaces__msg__IPASMeasurement__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ipas_interfaces__msg__IPASMeasurement__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ipas_interfaces__msg__IPASMeasurement__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for IPASMeasurement {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for IPASMeasurement where Self: Sized {
  const TYPE_NAME: &'static str = "ipas_interfaces/msg/IPASMeasurement";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ipas_interfaces__msg__IPASMeasurement() }
  }
}


