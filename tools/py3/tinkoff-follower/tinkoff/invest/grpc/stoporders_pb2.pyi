"""
@generated by mypy-protobuf.  Do not edit manually!
isort:skip_file
"""

import builtins
import collections.abc
import google.protobuf.descriptor
import google.protobuf.internal.containers
import google.protobuf.internal.enum_type_wrapper
import google.protobuf.message
import google.protobuf.timestamp_pb2
import sys
import tinkoff.invest.grpc.common_pb2
import typing

if sys.version_info >= (3, 10):
    import typing as typing_extensions
else:
    import typing_extensions

DESCRIPTOR: google.protobuf.descriptor.FileDescriptor

class _StopOrderDirection:
    ValueType = typing.NewType("ValueType", builtins.int)
    V: typing_extensions.TypeAlias = ValueType

class _StopOrderDirectionEnumTypeWrapper(google.protobuf.internal.enum_type_wrapper._EnumTypeWrapper[_StopOrderDirection.ValueType], builtins.type):
    DESCRIPTOR: google.protobuf.descriptor.EnumDescriptor
    STOP_ORDER_DIRECTION_UNSPECIFIED: _StopOrderDirection.ValueType  # 0
    """Значение не указано."""
    STOP_ORDER_DIRECTION_BUY: _StopOrderDirection.ValueType  # 1
    """Покупка."""
    STOP_ORDER_DIRECTION_SELL: _StopOrderDirection.ValueType  # 2
    """Продажа."""

class StopOrderDirection(_StopOrderDirection, metaclass=_StopOrderDirectionEnumTypeWrapper):
    """Направление сделки стоп-заявки."""

STOP_ORDER_DIRECTION_UNSPECIFIED: StopOrderDirection.ValueType  # 0
"""Значение не указано."""
STOP_ORDER_DIRECTION_BUY: StopOrderDirection.ValueType  # 1
"""Покупка."""
STOP_ORDER_DIRECTION_SELL: StopOrderDirection.ValueType  # 2
"""Продажа."""
global___StopOrderDirection = StopOrderDirection

class _StopOrderExpirationType:
    ValueType = typing.NewType("ValueType", builtins.int)
    V: typing_extensions.TypeAlias = ValueType

class _StopOrderExpirationTypeEnumTypeWrapper(google.protobuf.internal.enum_type_wrapper._EnumTypeWrapper[_StopOrderExpirationType.ValueType], builtins.type):
    DESCRIPTOR: google.protobuf.descriptor.EnumDescriptor
    STOP_ORDER_EXPIRATION_TYPE_UNSPECIFIED: _StopOrderExpirationType.ValueType  # 0
    """Значение не указано."""
    STOP_ORDER_EXPIRATION_TYPE_GOOD_TILL_CANCEL: _StopOrderExpirationType.ValueType  # 1
    """Действительно до отмены."""
    STOP_ORDER_EXPIRATION_TYPE_GOOD_TILL_DATE: _StopOrderExpirationType.ValueType  # 2
    """Действительно до даты снятия."""

class StopOrderExpirationType(_StopOrderExpirationType, metaclass=_StopOrderExpirationTypeEnumTypeWrapper):
    """Тип экспирации стоп-заявке."""

STOP_ORDER_EXPIRATION_TYPE_UNSPECIFIED: StopOrderExpirationType.ValueType  # 0
"""Значение не указано."""
STOP_ORDER_EXPIRATION_TYPE_GOOD_TILL_CANCEL: StopOrderExpirationType.ValueType  # 1
"""Действительно до отмены."""
STOP_ORDER_EXPIRATION_TYPE_GOOD_TILL_DATE: StopOrderExpirationType.ValueType  # 2
"""Действительно до даты снятия."""
global___StopOrderExpirationType = StopOrderExpirationType

class _StopOrderType:
    ValueType = typing.NewType("ValueType", builtins.int)
    V: typing_extensions.TypeAlias = ValueType

class _StopOrderTypeEnumTypeWrapper(google.protobuf.internal.enum_type_wrapper._EnumTypeWrapper[_StopOrderType.ValueType], builtins.type):
    DESCRIPTOR: google.protobuf.descriptor.EnumDescriptor
    STOP_ORDER_TYPE_UNSPECIFIED: _StopOrderType.ValueType  # 0
    """Значение не указано."""
    STOP_ORDER_TYPE_TAKE_PROFIT: _StopOrderType.ValueType  # 1
    """`Take-profit`-заявка."""
    STOP_ORDER_TYPE_STOP_LOSS: _StopOrderType.ValueType  # 2
    """`Stop-loss`-заявка."""
    STOP_ORDER_TYPE_STOP_LIMIT: _StopOrderType.ValueType  # 3
    """`Stop-limit`-заявка."""

class StopOrderType(_StopOrderType, metaclass=_StopOrderTypeEnumTypeWrapper):
    """Тип стоп-заявки."""

STOP_ORDER_TYPE_UNSPECIFIED: StopOrderType.ValueType  # 0
"""Значение не указано."""
STOP_ORDER_TYPE_TAKE_PROFIT: StopOrderType.ValueType  # 1
"""`Take-profit`-заявка."""
STOP_ORDER_TYPE_STOP_LOSS: StopOrderType.ValueType  # 2
"""`Stop-loss`-заявка."""
STOP_ORDER_TYPE_STOP_LIMIT: StopOrderType.ValueType  # 3
"""`Stop-limit`-заявка."""
global___StopOrderType = StopOrderType

class _StopOrderStatusOption:
    ValueType = typing.NewType("ValueType", builtins.int)
    V: typing_extensions.TypeAlias = ValueType

class _StopOrderStatusOptionEnumTypeWrapper(google.protobuf.internal.enum_type_wrapper._EnumTypeWrapper[_StopOrderStatusOption.ValueType], builtins.type):
    DESCRIPTOR: google.protobuf.descriptor.EnumDescriptor
    STOP_ORDER_STATUS_UNSPECIFIED: _StopOrderStatusOption.ValueType  # 0
    """Значение не указано."""
    STOP_ORDER_STATUS_ALL: _StopOrderStatusOption.ValueType  # 1
    """Все заявки."""
    STOP_ORDER_STATUS_ACTIVE: _StopOrderStatusOption.ValueType  # 2
    """Активные заявки."""
    STOP_ORDER_STATUS_EXECUTED: _StopOrderStatusOption.ValueType  # 3
    """Исполненные заявки."""
    STOP_ORDER_STATUS_CANCELED: _StopOrderStatusOption.ValueType  # 4
    """Отмененные заявки."""
    STOP_ORDER_STATUS_EXPIRED: _StopOrderStatusOption.ValueType  # 5
    """Истекшие заявки."""

class StopOrderStatusOption(_StopOrderStatusOption, metaclass=_StopOrderStatusOptionEnumTypeWrapper):
    """Статус стоп-заяки."""

STOP_ORDER_STATUS_UNSPECIFIED: StopOrderStatusOption.ValueType  # 0
"""Значение не указано."""
STOP_ORDER_STATUS_ALL: StopOrderStatusOption.ValueType  # 1
"""Все заявки."""
STOP_ORDER_STATUS_ACTIVE: StopOrderStatusOption.ValueType  # 2
"""Активные заявки."""
STOP_ORDER_STATUS_EXECUTED: StopOrderStatusOption.ValueType  # 3
"""Исполненные заявки."""
STOP_ORDER_STATUS_CANCELED: StopOrderStatusOption.ValueType  # 4
"""Отмененные заявки."""
STOP_ORDER_STATUS_EXPIRED: StopOrderStatusOption.ValueType  # 5
"""Истекшие заявки."""
global___StopOrderStatusOption = StopOrderStatusOption

class _ExchangeOrderType:
    ValueType = typing.NewType("ValueType", builtins.int)
    V: typing_extensions.TypeAlias = ValueType

class _ExchangeOrderTypeEnumTypeWrapper(google.protobuf.internal.enum_type_wrapper._EnumTypeWrapper[_ExchangeOrderType.ValueType], builtins.type):
    DESCRIPTOR: google.protobuf.descriptor.EnumDescriptor
    EXCHANGE_ORDER_TYPE_UNSPECIFIED: _ExchangeOrderType.ValueType  # 0
    """Значение не указано."""
    EXCHANGE_ORDER_TYPE_MARKET: _ExchangeOrderType.ValueType  # 1
    """Заявка по рыночной цене."""
    EXCHANGE_ORDER_TYPE_LIMIT: _ExchangeOrderType.ValueType  # 2
    """Лимитная заявка."""

class ExchangeOrderType(_ExchangeOrderType, metaclass=_ExchangeOrderTypeEnumTypeWrapper):
    """Тип выставляемой заявки."""

EXCHANGE_ORDER_TYPE_UNSPECIFIED: ExchangeOrderType.ValueType  # 0
"""Значение не указано."""
EXCHANGE_ORDER_TYPE_MARKET: ExchangeOrderType.ValueType  # 1
"""Заявка по рыночной цене."""
EXCHANGE_ORDER_TYPE_LIMIT: ExchangeOrderType.ValueType  # 2
"""Лимитная заявка."""
global___ExchangeOrderType = ExchangeOrderType

class _TakeProfitType:
    ValueType = typing.NewType("ValueType", builtins.int)
    V: typing_extensions.TypeAlias = ValueType

class _TakeProfitTypeEnumTypeWrapper(google.protobuf.internal.enum_type_wrapper._EnumTypeWrapper[_TakeProfitType.ValueType], builtins.type):
    DESCRIPTOR: google.protobuf.descriptor.EnumDescriptor
    TAKE_PROFIT_TYPE_UNSPECIFIED: _TakeProfitType.ValueType  # 0
    """Значение не указано."""
    TAKE_PROFIT_TYPE_REGULAR: _TakeProfitType.ValueType  # 1
    """Обычная заявка, значение по умолчанию."""
    TAKE_PROFIT_TYPE_TRAILING: _TakeProfitType.ValueType  # 2
    """Трейлинг-стоп."""

class TakeProfitType(_TakeProfitType, metaclass=_TakeProfitTypeEnumTypeWrapper):
    """Тип TakeProfit-заявки."""

TAKE_PROFIT_TYPE_UNSPECIFIED: TakeProfitType.ValueType  # 0
"""Значение не указано."""
TAKE_PROFIT_TYPE_REGULAR: TakeProfitType.ValueType  # 1
"""Обычная заявка, значение по умолчанию."""
TAKE_PROFIT_TYPE_TRAILING: TakeProfitType.ValueType  # 2
"""Трейлинг-стоп."""
global___TakeProfitType = TakeProfitType

class _TrailingValueType:
    ValueType = typing.NewType("ValueType", builtins.int)
    V: typing_extensions.TypeAlias = ValueType

class _TrailingValueTypeEnumTypeWrapper(google.protobuf.internal.enum_type_wrapper._EnumTypeWrapper[_TrailingValueType.ValueType], builtins.type):
    DESCRIPTOR: google.protobuf.descriptor.EnumDescriptor
    TRAILING_VALUE_UNSPECIFIED: _TrailingValueType.ValueType  # 0
    """Значение не указано."""
    TRAILING_VALUE_ABSOLUTE: _TrailingValueType.ValueType  # 1
    """Абсолютное значение в единицах цены."""
    TRAILING_VALUE_RELATIVE: _TrailingValueType.ValueType  # 2
    """Относительное значение в процентах."""

class TrailingValueType(_TrailingValueType, metaclass=_TrailingValueTypeEnumTypeWrapper):
    """Тип параметров значений трейлинг-стопа."""

TRAILING_VALUE_UNSPECIFIED: TrailingValueType.ValueType  # 0
"""Значение не указано."""
TRAILING_VALUE_ABSOLUTE: TrailingValueType.ValueType  # 1
"""Абсолютное значение в единицах цены."""
TRAILING_VALUE_RELATIVE: TrailingValueType.ValueType  # 2
"""Относительное значение в процентах."""
global___TrailingValueType = TrailingValueType

class _TrailingStopStatus:
    ValueType = typing.NewType("ValueType", builtins.int)
    V: typing_extensions.TypeAlias = ValueType

class _TrailingStopStatusEnumTypeWrapper(google.protobuf.internal.enum_type_wrapper._EnumTypeWrapper[_TrailingStopStatus.ValueType], builtins.type):
    DESCRIPTOR: google.protobuf.descriptor.EnumDescriptor
    TRAILING_STOP_UNSPECIFIED: _TrailingStopStatus.ValueType  # 0
    """Значение не указано."""
    TRAILING_STOP_ACTIVE: _TrailingStopStatus.ValueType  # 1
    """Активный."""
    TRAILING_STOP_ACTIVATED: _TrailingStopStatus.ValueType  # 2
    """Активированный."""

class TrailingStopStatus(_TrailingStopStatus, metaclass=_TrailingStopStatusEnumTypeWrapper):
    """Статус трейлинг-стопа."""

TRAILING_STOP_UNSPECIFIED: TrailingStopStatus.ValueType  # 0
"""Значение не указано."""
TRAILING_STOP_ACTIVE: TrailingStopStatus.ValueType  # 1
"""Активный."""
TRAILING_STOP_ACTIVATED: TrailingStopStatus.ValueType  # 2
"""Активированный."""
global___TrailingStopStatus = TrailingStopStatus

@typing.final
class PostStopOrderRequest(google.protobuf.message.Message):
    """Запрос выставления стоп-заявки."""

    DESCRIPTOR: google.protobuf.descriptor.Descriptor

    @typing.final
    class TrailingData(google.protobuf.message.Message):
        DESCRIPTOR: google.protobuf.descriptor.Descriptor

        INDENT_FIELD_NUMBER: builtins.int
        INDENT_TYPE_FIELD_NUMBER: builtins.int
        SPREAD_FIELD_NUMBER: builtins.int
        SPREAD_TYPE_FIELD_NUMBER: builtins.int
        indent_type: global___TrailingValueType.ValueType
        """Тип величины отступа."""
        spread_type: global___TrailingValueType.ValueType
        """Тип величины защитного спреда."""
        @property
        def indent(self) -> tinkoff.invest.grpc.common_pb2.Quotation:
            """Отступ."""

        @property
        def spread(self) -> tinkoff.invest.grpc.common_pb2.Quotation:
            """Размер защитного спреда."""

        def __init__(
            self,
            *,
            indent: tinkoff.invest.grpc.common_pb2.Quotation | None = ...,
            indent_type: global___TrailingValueType.ValueType = ...,
            spread: tinkoff.invest.grpc.common_pb2.Quotation | None = ...,
            spread_type: global___TrailingValueType.ValueType = ...,
        ) -> None: ...
        def HasField(self, field_name: typing.Literal["indent", b"indent", "spread", b"spread"]) -> builtins.bool: ...
        def ClearField(self, field_name: typing.Literal["indent", b"indent", "indent_type", b"indent_type", "spread", b"spread", "spread_type", b"spread_type"]) -> None: ...

    FIGI_FIELD_NUMBER: builtins.int
    QUANTITY_FIELD_NUMBER: builtins.int
    PRICE_FIELD_NUMBER: builtins.int
    STOP_PRICE_FIELD_NUMBER: builtins.int
    DIRECTION_FIELD_NUMBER: builtins.int
    ACCOUNT_ID_FIELD_NUMBER: builtins.int
    EXPIRATION_TYPE_FIELD_NUMBER: builtins.int
    STOP_ORDER_TYPE_FIELD_NUMBER: builtins.int
    EXPIRE_DATE_FIELD_NUMBER: builtins.int
    INSTRUMENT_ID_FIELD_NUMBER: builtins.int
    EXCHANGE_ORDER_TYPE_FIELD_NUMBER: builtins.int
    TAKE_PROFIT_TYPE_FIELD_NUMBER: builtins.int
    TRAILING_DATA_FIELD_NUMBER: builtins.int
    PRICE_TYPE_FIELD_NUMBER: builtins.int
    ORDER_ID_FIELD_NUMBER: builtins.int
    CONFIRM_MARGIN_TRADE_FIELD_NUMBER: builtins.int
    figi: builtins.str
    """Deprecated FIGI-идентификатор инструмента. Используйте `instrument_id`."""
    quantity: builtins.int
    """Количество лотов."""
    direction: global___StopOrderDirection.ValueType
    """Направление операции."""
    account_id: builtins.str
    """Номер счета."""
    expiration_type: global___StopOrderExpirationType.ValueType
    """Тип экспирации заявки."""
    stop_order_type: global___StopOrderType.ValueType
    """Тип заявки."""
    instrument_id: builtins.str
    """Идентификатор инструмента. Принимает значение `figi` или `instrument_uid`."""
    exchange_order_type: global___ExchangeOrderType.ValueType
    """Тип дочерней биржевой заявки."""
    take_profit_type: global___TakeProfitType.ValueType
    """Подтип стоп-заявки — `TakeProfit`."""
    price_type: tinkoff.invest.grpc.common_pb2.PriceType.ValueType
    """Тип цены."""
    order_id: builtins.str
    """Идентификатор запроса выставления поручения для целей идемпотентности в формате `UID`. Максимальная длина — 36 символов."""
    confirm_margin_trade: builtins.bool
    """Согласие на выставление заявки, которая может привести к непокрытой позиции, по умолчанию false."""
    @property
    def price(self) -> tinkoff.invest.grpc.common_pb2.Quotation:
        """Цена за 1 инструмент биржевой заявки, которая будет выставлена при срабатывании по достижению `stop_price`. Чтобы получить стоимость лота, нужно умножить на лотность инструмента."""

    @property
    def stop_price(self) -> tinkoff.invest.grpc.common_pb2.Quotation:
        """Стоп-цена заявки за 1 инструмент. При достижении стоп-цены происходит активация стоп-заявки, в результате чего выставляется биржевая заявка. Чтобы получить стоимость лота, нужно умножить на лотность инструмента."""

    @property
    def expire_date(self) -> google.protobuf.timestamp_pb2.Timestamp:
        """Дата и время окончания действия стоп-заявки по UTC. Для `ExpirationType = GoodTillDate` заполнение обязательно, для `GoodTillCancel` игнорируется."""

    @property
    def trailing_data(self) -> global___PostStopOrderRequest.TrailingData:
        """Массив с параметрами трейлинг-стопа."""

    def __init__(
        self,
        *,
        figi: builtins.str | None = ...,
        quantity: builtins.int = ...,
        price: tinkoff.invest.grpc.common_pb2.Quotation | None = ...,
        stop_price: tinkoff.invest.grpc.common_pb2.Quotation | None = ...,
        direction: global___StopOrderDirection.ValueType = ...,
        account_id: builtins.str = ...,
        expiration_type: global___StopOrderExpirationType.ValueType = ...,
        stop_order_type: global___StopOrderType.ValueType = ...,
        expire_date: google.protobuf.timestamp_pb2.Timestamp | None = ...,
        instrument_id: builtins.str = ...,
        exchange_order_type: global___ExchangeOrderType.ValueType = ...,
        take_profit_type: global___TakeProfitType.ValueType = ...,
        trailing_data: global___PostStopOrderRequest.TrailingData | None = ...,
        price_type: tinkoff.invest.grpc.common_pb2.PriceType.ValueType = ...,
        order_id: builtins.str = ...,
        confirm_margin_trade: builtins.bool = ...,
    ) -> None: ...
    def HasField(self, field_name: typing.Literal["_expire_date", b"_expire_date", "_figi", b"_figi", "_price", b"_price", "_stop_price", b"_stop_price", "expire_date", b"expire_date", "figi", b"figi", "price", b"price", "stop_price", b"stop_price", "trailing_data", b"trailing_data"]) -> builtins.bool: ...
    def ClearField(self, field_name: typing.Literal["_expire_date", b"_expire_date", "_figi", b"_figi", "_price", b"_price", "_stop_price", b"_stop_price", "account_id", b"account_id", "confirm_margin_trade", b"confirm_margin_trade", "direction", b"direction", "exchange_order_type", b"exchange_order_type", "expiration_type", b"expiration_type", "expire_date", b"expire_date", "figi", b"figi", "instrument_id", b"instrument_id", "order_id", b"order_id", "price", b"price", "price_type", b"price_type", "quantity", b"quantity", "stop_order_type", b"stop_order_type", "stop_price", b"stop_price", "take_profit_type", b"take_profit_type", "trailing_data", b"trailing_data"]) -> None: ...
    @typing.overload
    def WhichOneof(self, oneof_group: typing.Literal["_expire_date", b"_expire_date"]) -> typing.Literal["expire_date"] | None: ...
    @typing.overload
    def WhichOneof(self, oneof_group: typing.Literal["_figi", b"_figi"]) -> typing.Literal["figi"] | None: ...
    @typing.overload
    def WhichOneof(self, oneof_group: typing.Literal["_price", b"_price"]) -> typing.Literal["price"] | None: ...
    @typing.overload
    def WhichOneof(self, oneof_group: typing.Literal["_stop_price", b"_stop_price"]) -> typing.Literal["stop_price"] | None: ...

global___PostStopOrderRequest = PostStopOrderRequest

@typing.final
class PostStopOrderResponse(google.protobuf.message.Message):
    """Результат выставления стоп-заявки."""

    DESCRIPTOR: google.protobuf.descriptor.Descriptor

    STOP_ORDER_ID_FIELD_NUMBER: builtins.int
    ORDER_REQUEST_ID_FIELD_NUMBER: builtins.int
    RESPONSE_METADATA_FIELD_NUMBER: builtins.int
    stop_order_id: builtins.str
    """Уникальный идентификатор стоп-заявки."""
    order_request_id: builtins.str
    """Идентификатор ключа идемпотентности, переданный клиентом, в формате `UID`. Максимальная длина 36 — символов."""
    @property
    def response_metadata(self) -> tinkoff.invest.grpc.common_pb2.ResponseMetadata:
        """Метадата."""

    def __init__(
        self,
        *,
        stop_order_id: builtins.str = ...,
        order_request_id: builtins.str = ...,
        response_metadata: tinkoff.invest.grpc.common_pb2.ResponseMetadata | None = ...,
    ) -> None: ...
    def HasField(self, field_name: typing.Literal["response_metadata", b"response_metadata"]) -> builtins.bool: ...
    def ClearField(self, field_name: typing.Literal["order_request_id", b"order_request_id", "response_metadata", b"response_metadata", "stop_order_id", b"stop_order_id"]) -> None: ...

global___PostStopOrderResponse = PostStopOrderResponse

@typing.final
class GetStopOrdersRequest(google.protobuf.message.Message):
    """Запрос получения списка активных стоп-заявок."""

    DESCRIPTOR: google.protobuf.descriptor.Descriptor

    ACCOUNT_ID_FIELD_NUMBER: builtins.int
    STATUS_FIELD_NUMBER: builtins.int
    FROM_FIELD_NUMBER: builtins.int
    TO_FIELD_NUMBER: builtins.int
    account_id: builtins.str
    """Идентификатор счета клиента."""
    status: global___StopOrderStatusOption.ValueType
    """Статус заявок."""
    @property
    def to(self) -> google.protobuf.timestamp_pb2.Timestamp:
        """Правая граница."""

    def __init__(
        self,
        *,
        account_id: builtins.str = ...,
        status: global___StopOrderStatusOption.ValueType = ...,
        to: google.protobuf.timestamp_pb2.Timestamp | None = ...,
    ) -> None: ...
    def HasField(self, field_name: typing.Literal["from", b"from", "to", b"to"]) -> builtins.bool: ...
    def ClearField(self, field_name: typing.Literal["account_id", b"account_id", "from", b"from", "status", b"status", "to", b"to"]) -> None: ...

global___GetStopOrdersRequest = GetStopOrdersRequest

@typing.final
class GetStopOrdersResponse(google.protobuf.message.Message):
    """Список активных стоп-заявок."""

    DESCRIPTOR: google.protobuf.descriptor.Descriptor

    STOP_ORDERS_FIELD_NUMBER: builtins.int
    @property
    def stop_orders(self) -> google.protobuf.internal.containers.RepeatedCompositeFieldContainer[global___StopOrder]:
        """Массив стоп-заявок по счету."""

    def __init__(
        self,
        *,
        stop_orders: collections.abc.Iterable[global___StopOrder] | None = ...,
    ) -> None: ...
    def ClearField(self, field_name: typing.Literal["stop_orders", b"stop_orders"]) -> None: ...

global___GetStopOrdersResponse = GetStopOrdersResponse

@typing.final
class CancelStopOrderRequest(google.protobuf.message.Message):
    """Запрос отмены выставленной стоп-заявки."""

    DESCRIPTOR: google.protobuf.descriptor.Descriptor

    ACCOUNT_ID_FIELD_NUMBER: builtins.int
    STOP_ORDER_ID_FIELD_NUMBER: builtins.int
    account_id: builtins.str
    """Идентификатор счета клиента."""
    stop_order_id: builtins.str
    """Уникальный идентификатор стоп-заявки."""
    def __init__(
        self,
        *,
        account_id: builtins.str = ...,
        stop_order_id: builtins.str = ...,
    ) -> None: ...
    def ClearField(self, field_name: typing.Literal["account_id", b"account_id", "stop_order_id", b"stop_order_id"]) -> None: ...

global___CancelStopOrderRequest = CancelStopOrderRequest

@typing.final
class CancelStopOrderResponse(google.protobuf.message.Message):
    """Результат отмены выставленной стоп-заявки."""

    DESCRIPTOR: google.protobuf.descriptor.Descriptor

    TIME_FIELD_NUMBER: builtins.int
    @property
    def time(self) -> google.protobuf.timestamp_pb2.Timestamp:
        """Время отмены заявки по UTC."""

    def __init__(
        self,
        *,
        time: google.protobuf.timestamp_pb2.Timestamp | None = ...,
    ) -> None: ...
    def HasField(self, field_name: typing.Literal["time", b"time"]) -> builtins.bool: ...
    def ClearField(self, field_name: typing.Literal["time", b"time"]) -> None: ...

global___CancelStopOrderResponse = CancelStopOrderResponse

@typing.final
class StopOrder(google.protobuf.message.Message):
    """Информация о стоп-заявке."""

    DESCRIPTOR: google.protobuf.descriptor.Descriptor

    @typing.final
    class TrailingData(google.protobuf.message.Message):
        DESCRIPTOR: google.protobuf.descriptor.Descriptor

        INDENT_FIELD_NUMBER: builtins.int
        INDENT_TYPE_FIELD_NUMBER: builtins.int
        SPREAD_FIELD_NUMBER: builtins.int
        SPREAD_TYPE_FIELD_NUMBER: builtins.int
        STATUS_FIELD_NUMBER: builtins.int
        PRICE_FIELD_NUMBER: builtins.int
        EXTR_FIELD_NUMBER: builtins.int
        indent_type: global___TrailingValueType.ValueType
        """Тип величины отступа."""
        spread_type: global___TrailingValueType.ValueType
        """Тип величины защитного спреда."""
        status: global___TrailingStopStatus.ValueType
        """Статус трейлинг-стопа."""
        @property
        def indent(self) -> tinkoff.invest.grpc.common_pb2.Quotation:
            """Отступ."""

        @property
        def spread(self) -> tinkoff.invest.grpc.common_pb2.Quotation:
            """Размер защитного спреда."""

        @property
        def price(self) -> tinkoff.invest.grpc.common_pb2.Quotation:
            """Цена исполнения."""

        @property
        def extr(self) -> tinkoff.invest.grpc.common_pb2.Quotation:
            """Локальный экстремум."""

        def __init__(
            self,
            *,
            indent: tinkoff.invest.grpc.common_pb2.Quotation | None = ...,
            indent_type: global___TrailingValueType.ValueType = ...,
            spread: tinkoff.invest.grpc.common_pb2.Quotation | None = ...,
            spread_type: global___TrailingValueType.ValueType = ...,
            status: global___TrailingStopStatus.ValueType = ...,
            price: tinkoff.invest.grpc.common_pb2.Quotation | None = ...,
            extr: tinkoff.invest.grpc.common_pb2.Quotation | None = ...,
        ) -> None: ...
        def HasField(self, field_name: typing.Literal["extr", b"extr", "indent", b"indent", "price", b"price", "spread", b"spread"]) -> builtins.bool: ...
        def ClearField(self, field_name: typing.Literal["extr", b"extr", "indent", b"indent", "indent_type", b"indent_type", "price", b"price", "spread", b"spread", "spread_type", b"spread_type", "status", b"status"]) -> None: ...

    STOP_ORDER_ID_FIELD_NUMBER: builtins.int
    LOTS_REQUESTED_FIELD_NUMBER: builtins.int
    FIGI_FIELD_NUMBER: builtins.int
    DIRECTION_FIELD_NUMBER: builtins.int
    CURRENCY_FIELD_NUMBER: builtins.int
    ORDER_TYPE_FIELD_NUMBER: builtins.int
    CREATE_DATE_FIELD_NUMBER: builtins.int
    ACTIVATION_DATE_TIME_FIELD_NUMBER: builtins.int
    EXPIRATION_TIME_FIELD_NUMBER: builtins.int
    PRICE_FIELD_NUMBER: builtins.int
    STOP_PRICE_FIELD_NUMBER: builtins.int
    INSTRUMENT_UID_FIELD_NUMBER: builtins.int
    TAKE_PROFIT_TYPE_FIELD_NUMBER: builtins.int
    TRAILING_DATA_FIELD_NUMBER: builtins.int
    STATUS_FIELD_NUMBER: builtins.int
    EXCHANGE_ORDER_TYPE_FIELD_NUMBER: builtins.int
    EXCHANGE_ORDER_ID_FIELD_NUMBER: builtins.int
    stop_order_id: builtins.str
    """Уникальный идентификатор стоп-заявки."""
    lots_requested: builtins.int
    """Запрошено лотов."""
    figi: builtins.str
    """FIGI-идентификатор инструмента."""
    direction: global___StopOrderDirection.ValueType
    """Направление операции."""
    currency: builtins.str
    """Валюта стоп-заявки."""
    order_type: global___StopOrderType.ValueType
    """Тип стоп-заявки."""
    instrument_uid: builtins.str
    """`instrument_uid`-идентификатор инструмента."""
    take_profit_type: global___TakeProfitType.ValueType
    """Подтип стоп-заявки — `TakeProfit`."""
    status: global___StopOrderStatusOption.ValueType
    """Статус заявки."""
    exchange_order_type: global___ExchangeOrderType.ValueType
    """Тип дочерней биржевой заявки для тейкпрофита."""
    exchange_order_id: builtins.str
    """Идентификатор биржевой заявки."""
    @property
    def create_date(self) -> google.protobuf.timestamp_pb2.Timestamp:
        """Дата и время выставления заявки по UTC."""

    @property
    def activation_date_time(self) -> google.protobuf.timestamp_pb2.Timestamp:
        """Дата и время конвертации стоп-заявки в биржевую по UTC."""

    @property
    def expiration_time(self) -> google.protobuf.timestamp_pb2.Timestamp:
        """Дата и время снятия заявки по UTC."""

    @property
    def price(self) -> tinkoff.invest.grpc.common_pb2.MoneyValue:
        """Цена заявки за 1 инструмент. Чтобы получить стоимость лота, нужно умножить на лотность инструмента."""

    @property
    def stop_price(self) -> tinkoff.invest.grpc.common_pb2.MoneyValue:
        """Цена активации стоп-заявки за 1 инструмент. Чтобы получить стоимость лота, нужно умножить на лотность инструмента."""

    @property
    def trailing_data(self) -> global___StopOrder.TrailingData:
        """Параметры трейлинг-стопа."""

    def __init__(
        self,
        *,
        stop_order_id: builtins.str = ...,
        lots_requested: builtins.int = ...,
        figi: builtins.str = ...,
        direction: global___StopOrderDirection.ValueType = ...,
        currency: builtins.str = ...,
        order_type: global___StopOrderType.ValueType = ...,
        create_date: google.protobuf.timestamp_pb2.Timestamp | None = ...,
        activation_date_time: google.protobuf.timestamp_pb2.Timestamp | None = ...,
        expiration_time: google.protobuf.timestamp_pb2.Timestamp | None = ...,
        price: tinkoff.invest.grpc.common_pb2.MoneyValue | None = ...,
        stop_price: tinkoff.invest.grpc.common_pb2.MoneyValue | None = ...,
        instrument_uid: builtins.str = ...,
        take_profit_type: global___TakeProfitType.ValueType = ...,
        trailing_data: global___StopOrder.TrailingData | None = ...,
        status: global___StopOrderStatusOption.ValueType = ...,
        exchange_order_type: global___ExchangeOrderType.ValueType = ...,
        exchange_order_id: builtins.str | None = ...,
    ) -> None: ...
    def HasField(self, field_name: typing.Literal["_exchange_order_id", b"_exchange_order_id", "activation_date_time", b"activation_date_time", "create_date", b"create_date", "exchange_order_id", b"exchange_order_id", "expiration_time", b"expiration_time", "price", b"price", "stop_price", b"stop_price", "trailing_data", b"trailing_data"]) -> builtins.bool: ...
    def ClearField(self, field_name: typing.Literal["_exchange_order_id", b"_exchange_order_id", "activation_date_time", b"activation_date_time", "create_date", b"create_date", "currency", b"currency", "direction", b"direction", "exchange_order_id", b"exchange_order_id", "exchange_order_type", b"exchange_order_type", "expiration_time", b"expiration_time", "figi", b"figi", "instrument_uid", b"instrument_uid", "lots_requested", b"lots_requested", "order_type", b"order_type", "price", b"price", "status", b"status", "stop_order_id", b"stop_order_id", "stop_price", b"stop_price", "take_profit_type", b"take_profit_type", "trailing_data", b"trailing_data"]) -> None: ...
    def WhichOneof(self, oneof_group: typing.Literal["_exchange_order_id", b"_exchange_order_id"]) -> typing.Literal["exchange_order_id"] | None: ...

global___StopOrder = StopOrder
