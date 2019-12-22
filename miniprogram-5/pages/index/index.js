//js
Page({
  /**
     * 页面的初始数据
     */
  data: {
    device_name:32,
    latitude: '',
    longitude: '',
  },
  GPSsubmit: function (e) {
    wx.getLocation({
      type: 'wgs84',
      success: (res) => {
        var latitude = res.latitude
        var longitude = res.longitude
        var speed = res.speed
        var accuracy = res.accuracy
        this.setData({ latitude: latitude, longitude: longitude })
        wx.showModal({
          title: '当前位置',
          content: '经度' + res.longitude + '纬度' + res.latitude,
        })
      }

    })
  },//获取经度和纬度
  onLoad() {
   
      var _this = this;
      //console.log(options.device_id);// 打印设备ID
      //console.log(app.data.api_key);// 打印API_KEY
      wx.request({
        url: 'https://api.heclouds.com/devices/562234373/datapoints',
        header: {
          'api-key': 'Rh3VomUuIy1yXEpJURmUlx7npRE='
        },
        success: function (res) {
          console.log(res.data);
          _this.setData(
            {
              device_name: res.data.data.datastreams[0].datapoints[0].value,

            });
        },
        fail: function () {
          wx.showToast({
            title: '与服务器通信失败',
            icon: 'fail',
            duration: 2000
          })
        }
      })
      //实时刷新我们的地理位置
    wx.getLocation({
      type: 'gcj02',
      success: (res) => {
        console.log(res)
        this.setData({
          latitude: res.latitude,
          longitude: res.longitude
        })
      }
    })
  },
})
