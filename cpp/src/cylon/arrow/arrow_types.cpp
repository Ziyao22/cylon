/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cylon/arrow/arrow_types.hpp>
#include <cylon/data_types.hpp>

#include <glog/logging.h>

namespace cylon {
namespace tarrow {

std::shared_ptr<arrow::DataType> convertToArrowType(const std::shared_ptr<DataType> &tType, int32_t width) {
  switch (tType->getType()) {
    case Type::BOOL:return std::make_shared<arrow::BooleanType>();
    case Type::UINT8:return std::make_shared<arrow::UInt8Type>();
    case Type::INT8:return std::make_shared<arrow::Int8Type>();
    case Type::UINT16:return std::make_shared<arrow::UInt16Type>();
    case Type::INT16:return std::make_shared<arrow::Int16Type>();
    case Type::UINT32:return std::make_shared<arrow::UInt32Type>();
    case Type::INT32:return std::make_shared<arrow::Int32Type>();
    case Type::UINT64:return std::make_shared<arrow::UInt64Type>();
    case Type::INT64:return std::make_shared<arrow::Int64Type>();
    case Type::HALF_FLOAT:return std::make_shared<arrow::HalfFloatType>();
    case Type::FLOAT:return std::make_shared<arrow::FloatType>();
    case Type::DOUBLE:return std::make_shared<arrow::DoubleType>();
    case Type::STRING:return std::make_shared<arrow::StringType>();
    case Type::BINARY:return std::make_shared<arrow::BinaryType>();
    case Type::FIXED_SIZE_BINARY: {
      if (width < 0) break;
      return std::make_shared<arrow::FixedSizeBinaryType>(width);
    }
    case Type::DATE32:return std::make_shared<arrow::Date32Type>();
    case Type::DATE64:return std::make_shared<arrow::Date64Type>();
    case Type::TIMESTAMP:return std::make_shared<arrow::TimestampType>();
    case Type::TIME32:return std::make_shared<arrow::Time32Type>();
    case Type::TIME64:return std::make_shared<arrow::Time64Type>();
    case Type::DURATION:return std::make_shared<arrow::DurationType>();
    case Type::DECIMAL: break;
    case Type::INTERVAL:break;
    case Type::LIST:break;
    case Type::FIXED_SIZE_LIST:break;
    case Type::EXTENSION:break;
  }
  return nullptr;
}

bool validateArrowTableTypes(const std::shared_ptr<arrow::Table> &table) {
  return CheckSupportedTypes(table).is_ok();
}

Status CheckSupportedTypes(const std::shared_ptr<arrow::Table> &table) {
  const auto &schema = table->schema();
  for (const auto &t: schema->fields()) {
    switch (t->type()->id()) {
      /* following types are supported. go to next column type */
      case arrow::Type::BOOL:
      case arrow::Type::UINT8:
      case arrow::Type::INT8:
      case arrow::Type::UINT16:
      case arrow::Type::INT16:
      case arrow::Type::UINT32:
      case arrow::Type::INT32:
      case arrow::Type::UINT64:
      case arrow::Type::INT64:
      case arrow::Type::HALF_FLOAT:
      case arrow::Type::FLOAT:
      case arrow::Type::DOUBLE:
      case arrow::Type::BINARY:
      case arrow::Type::FIXED_SIZE_BINARY:
      case arrow::Type::STRING:
      case arrow::Type::DATE32:
      case arrow::Type::DATE64:
      case arrow::Type::TIMESTAMP:
      case arrow::Type::TIME32:
      case arrow::Type::TIME64: continue;
      case arrow::Type::LIST: {
        const auto &t_value = std::static_pointer_cast<arrow::ListType>(t->type());
        switch (t_value->value_type()->id()) {
          /* following types are supported. go to next column type */
          case arrow::Type::UINT8:
          case arrow::Type::INT8:
          case arrow::Type::UINT16:
          case arrow::Type::INT16:
          case arrow::Type::UINT32:
          case arrow::Type::INT32:
          case arrow::Type::UINT64:
          case arrow::Type::INT64:
          case arrow::Type::HALF_FLOAT:
          case arrow::Type::FLOAT:
          case arrow::Type::DOUBLE:continue;
          default:
            return {Code::NotImplemented, "unsupported value type for lists " + t_value->value_type()->ToString()};;
        }
      }
      default: return {Code::NotImplemented, "unsupported type " + t->type()->ToString()};
    }
  }
  return Status::OK();
}

std::shared_ptr<DataType> ToCylonType(const std::shared_ptr<arrow::DataType> &arr_type) {
  switch (arr_type->id()) {
    case arrow::Type::BOOL:return cylon::Bool();
    case arrow::Type::UINT8:return cylon::UInt8();
    case arrow::Type::INT8:return cylon::Int8();
    case arrow::Type::UINT16:return cylon::UInt16();
    case arrow::Type::INT16:return cylon::Int16();
    case arrow::Type::UINT32:return cylon::UInt32();
    case arrow::Type::INT32:return cylon::Int32();
    case arrow::Type::UINT64:return cylon::UInt64();
    case arrow::Type::INT64:return cylon::Int64();
    case arrow::Type::HALF_FLOAT:return cylon::HalfFloat();
    case arrow::Type::FLOAT:return cylon::Float();
    case arrow::Type::DOUBLE:return cylon::Double();
    case arrow::Type::BINARY:return cylon::Binary();
    case arrow::Type::FIXED_SIZE_BINARY:return cylon::FixedBinary();
    case arrow::Type::STRING:return cylon::String();
    case arrow::Type::DATE32:return cylon::Date32();
    case arrow::Type::DATE64:return cylon::Date64();
    case arrow::Type::TIMESTAMP:return cylon::Timestamp();
    case arrow::Type::TIME32:return cylon::Time32();
    case arrow::Type::TIME64:return cylon::Time64();
    case arrow::Type::DECIMAL:return cylon::Decimal();
    case arrow::Type::NA:break;
    case arrow::Type::INTERVAL_MONTHS:break;
    case arrow::Type::INTERVAL_DAY_TIME:break;
    case arrow::Type::LIST:break;
    case arrow::Type::STRUCT:break;
    case arrow::Type::SPARSE_UNION:break;
    case arrow::Type::DENSE_UNION:break;
    case arrow::Type::DICTIONARY:break;
    case arrow::Type::MAP:break;
    case arrow::Type::EXTENSION:break;
    case arrow::Type::FIXED_SIZE_LIST:break;
    case arrow::Type::DURATION:break;
    case arrow::Type::LARGE_STRING:break;
    case arrow::Type::LARGE_BINARY:break;
    case arrow::Type::LARGE_LIST:break;
    case arrow::Type::MAX_ID:break;
    case arrow::Type::DECIMAL256:break;
  }
  return nullptr;
}

}  // namespace tarrow
}  // namespace cylon
